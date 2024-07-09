// go.c

inherit F_CLEAN_UP;

mapping default_dirs = ([
	"north":	"北面",
	"south":	"南面",
	"east":		"东面",
	"west":		"西面",
	"northup":      "北边",
	"southup":      "南边",
	"eastup":       "东边",
	"westup":       "西边",
	"northdown":    "北边",
	"southdown":    "南边",
	"eastdown":     "东边",
	"westdown":     "西边",
	"northeast":    "东北",
	"northwest":    "西北",
	"southeast":    "东南",
	"southwest":    "西南",
	"up":	   "上面",
	"down":	 "下面",
	"enter":	"里面",
	"out":	  "外面",
]);

int night = 0;

void do_flee(object me);

int night_time(int i)
{
    night = i;
    return night;
}

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
	string dest, mout, min, dir;
	int cost;
	object env, obj;
	object *f_obs;
	mapping dirs;

	if (!arg ) return notify_fail("你要往哪个方向走？\n");
	
	if(arg=="away") {
		if(!me->is_fighting()) return notify_fail("你又不在战斗中跑什么跑？\n");
		do_flee(me);
		return 1;
	}
		
	if (me->query_temp("on_baitan") )
		return notify_fail("你正在摆摊，起身不得。\n");
		
        if (me->over_encumbranced() && userp(me) )
		return notify_fail("你的负荷过重，动弹不得。\n");

	if (me->is_busy() )
		return notify_fail("你的动作还没有完成，不能移动。\n");
	env = environment(me);
	
	if(IN_MAIN) {
		if(CP_D->enter(me,env,arg)) return 1;
	}
	else if(CP_D->exit(me,env,arg)) return 1;
	
	if (me->is_fighting()) {
		f_obs = me->query_enemy();
		if (objectp(f_obs[0]) && living(f_obs[0]))
		if (env == environment(f_obs[0]) && me->query_dex(2)/4 <= 
			random((int)f_obs[0]->query_dex(2))) {
			me->start_busy(5);
			if(f_obs[0]->query("race") != "人类")
			       message_vision("$N转身想跑，被$n猛地一下扑上来，拦住去路！\n", me, f_obs[0]);
			else message_vision("$N转身就要开溜，被$n一把拦住！\n", me, f_obs[0]);
			return notify_fail("你逃跑失败。\n");
		}
	}

	if (!env) return notify_fail("你哪里也去不了。\n");

	dest = env->query("exits/"+arg);
	if (!dest) {
		if (query_verb()=="go") notify_fail("这个方向没有出路。\n");
		return 0;
	}
        dirs = env->query("exits");
        if (!mapp(dirs) || !(dest = dirs[arg])) {
                if (query_verb()=="go") notify_fail("这个方向没有出路。\n");
                return 0;
        }

	obj = load_object(dest);
	if (!obj) return notify_fail("这个方向无法移动，请报告巫师。\n");

	if (!env->valid_leave(me, arg) ) {
		if (this_player() == me) write(query_notify_fail());
		return 1;
	}

	if (env->query("outdoors")) cost = 1 + random(me->query_encumbrance() / 20000);
	cost += env->query("jingli_cost/"+arg);
	if (NATURE_D->is_night()) cost *= 2;
	if (userp(me) && !wizardp(me) && me->query("jingli") < cost)
		return notify_fail("你太累了，休息一下再走吧。\n");

	dir = obj->query("short");
	
	if (obj->query("daytime_only") && NATURE_D->is_night()){
		if(stringp(min = obj->query("daytime_only"))) return notify_fail(min);
		return notify_fail(dir+"已经关门了，你等天亮了再来吧！\n");
	}

	if (!undefinedp(default_dirs[arg]))
		dir = default_dirs[arg] + "的" + dir;

	if (me->is_fighting() && !me->clean_up_enemy() && me->is_fighting()) {
		mout = "往" + dir + "落荒而逃了。\n";
		min = "从" + env->query("short") + "跌跌撞撞地跑了过来，模样有些狼狈。\n";
	} else {
		min = mout = me->query_condition("killer")?
			"神色慌张地":"";
		mout += "往" + dir + "离开。\n";
		switch(me->query("race")) {
			case "人类":
				min += "从" + env->query("short") + "走了过来。\n";
				break;
			default:
				min = "从" + env->query("short") + "蹿了过来。\n";
		}
	}

        if (me->query_condition("no_force")) me->start_busy(1);

	if (me->move(obj) ) {
		// night effect, added by snomwan@SJ
/*
		if(NATURE_D->is_night() || env->query("no_light")){
			f_obs = all_inventory(me);
			if (sizeof(f_obs))
			foreach (object item in f_obs) 
				if(item->query("light/on")){
					if((env->query("outdoors") || env->query("no_light")) // for some special rooms.
					&& env->query_temp("light_id") == item
					&& env->query_night_light()){
						env->light_gone(); // lights gone.
						env->delete_temp("light_id");
						item->delete("light/env");
						mout += "带走了"+item->name()+"，这里迅速暗淡了下来。\n";
					}
					if((obj->query("outdoors") || obj->query("no_light"))
					&& !obj->query_night_light()){
						obj->light_up();  // i bring in light :)
						obj->set_temp("light_id", item);
						item->set("light/env", file_name(obj));
						min += "带来的"+item->name()+"一下子将这里照亮了。\n";
					}
			}
		}
*/
		if ( time()-me->query_temp("last_go") > 1	|| !userp(me))
			tell_room(env, me->name() + mout, ({me}) );
		if (cost > 0 && userp(me) && !wizardp(me))
			me->receive_damage("jingli", cost);
		me->remove_all_enemy();
		if ( time()-me->query_temp("last_go") > 1	|| !userp(me))
			tell_room(obj, me->name() + min, ({me}) );
		me->set_temp("last_go",time());
		if (userp(me) && !obj->query("no_save") && !me->is_ghost() && !clonep(obj))	
		  me->set("startroom", base_name(obj));
	
		me->delete_temp("pending");
		me->set_temp("perform_AP",0);

		all_inventory(env)->follow_me(me, arg);
		if (!userp(me)) {
			if (stringp(dest = me->query("startroom")) && !me->query("location")) {
//				min = implode(explode(dest, "/")[0..<2], "/");
				if (random(me->query_temp("random_move")) > 15)
//				|| strsrch(base_name(environment(me)), min) != 1)
					me->return_home(dest);
			}
		}
		return 1;
	}
	return 0;
}

void do_flee(object me)
{
	mapping exits;
	string *directions;

	if (!environment(me) || !living(me) ) return;
	exits = environment(me)->query("exits");
	if (!mapp(exits) || !sizeof(exits) ) return;
	directions = keys(exits);
	tell_object(me, "看来该找机会逃跑了...\n");
	main(me, directions[random(sizeof(directions))]);
}

int help(object me)
{
	write(@HELP
指令格式 : go <方向>
 
让你往指定的方向移动。
 
HELP
    );
    return 1;
}
