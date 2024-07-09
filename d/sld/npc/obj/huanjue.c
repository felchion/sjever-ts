// huanjue
// 99.10.15  by emnil

#include <ansi.h>

inherit ITEM;

void create()
{
	set_name("", ({ "huanjue"}) );
	set("long","");
	set("unit", "只");
	set("weight", 0);
	set("no_sell",1);
	set("no_get",1);
	set("no_steal",1);
	set("no_drop",1);
	set("no_give",1);
	set("value", 1);
}

void init()
{
	add_action("do_look","look");
	add_action("do_go","go");
	add_action("do_dir",({"e","east","w","west","n","north","s","south","ne","northeast","nw","northwest",
			      "se","southeast","sw","southwest","nu","northup","su","southup","eu","eastup",
			      "wu","westup","nd","northdown","sd","southdown","ed","eastdown","wd","westdown",
			      "up","down","out","enter"}));
	add_action("do_i","i");
	add_action("do_i","inventory");
	add_action("do_id","id");
	add_action("do_dummy",({"follow","tell","reply","say","chat","say","party","rumor"}));
	this_player()->delete_temp("follow");
}

int do_dummy(string arg)
{
	return 1;
}

int do_go(string arg)
{
	mapping dirs = ([
	"north":		"北面",
	"south":		"南面",
	"east":			"东面",
	"west":			"西面",
	"northup":		"北边",
	"southup":		"南边",
	"eastup":		"东边",
	"westup":		"西边",
	"northdown":	"北边",
	"southdown":	"南边",
	"eastdown":		"东边",
	"westdown":		"西边",
	"northeast":	"东北",
	"northwest":	"西北",
	"southeast":	"东南",
	"southwest":	"西南",
	"up":			"上面",
	"down":			"下面",
	"enter":		"里面",
	"out":			"外面",
	]);
	string dest, mout, min, dir;
	object env, obj;
	mapping exit;
	object *f_obs;
	object me=this_player();

	if( !arg ) return notify_fail("你要往哪个方向走？\n");

	if( me->over_encumbranced() )
		return notify_fail("你的负荷过重，动弹不得。\n");

	if( me->is_busy() )
		return notify_fail("你的动作还没有完成，不能移动。\n");
	if (me->is_fighting()) {
		f_obs = me->query_enemy();
		if (objectp(f_obs[0]))
		if (2 + random((int)me->query_dex()) <= 
			random((int)f_obs[0]->query_dex())) {
			me->start_busy(1);
			return notify_fail("你逃跑失败。\n");
		}
	}

	env = environment(me);
	if(!env) return notify_fail("你哪里也去不了。\n");

	if( !mapp(exit = env->query("exits")) || undefinedp(exit[arg]) ) {
		if( query_verb()=="go")
			return notify_fail("这个方向没有出路。\n");
		else
			return 0;
	}

	dest = exit[arg];

	if( !(obj = find_object(dest)) )
		call_other(dest, "???");
	if( !(obj = find_object(dest)) )
		return notify_fail("无法移动。\n");

	if( !env->valid_leave(me, arg) ) {
		if (this_player() == me)
			write(query_notify_fail());
		return 1;
	}

	if (env->query("outdoors")
	&& userp(me) && !wizardp(me) && me->query("jingli") < 1)
		return notify_fail("你太累了，休息一下再走吧。\n");

	dir = obj->query("short");
	if (!undefinedp(dirs[arg]))
		dir = dirs[arg] + "的" + dir;

	mout = "";
	min = "";
	if( me->is_fighting() ) {
		mout += "往" + dir + "落荒而逃了。\n";
		min += "从" + env->query("short") + "跌跌撞撞地跑了过来，模样有些狼狈。\n";
	} else {
		mout += me->query_condition("killer")?
			"神色慌张地":"";
		mout += "往" + dir;
		mout += env->query("outdoors")?"快步":"";
		mout += "离开。\n";
		min += me->query_condition("killer")?"神色慌张地":"";
		min += "从" + env->query("short");
		min += env->query("outdoors")?"快步":"";
		min += "走了过来。\n";
	}

	if (!wizardp(me) || !me->query("env/invisibility"))
		tell_room(env, me->name() + replace_string(mout, "$n", ""), ({me}) );
	if (env->query("outdoors") && userp(me) && !wizardp(me))
		me->receive_damage("jingli", 1);
	if( me->move_object(obj) ) {
		env->add_encumbrance(-me->weight());
		obj->add_encumbrance(me->weight());
		me->remove_all_enemy();
		if (!wizardp(me) || !me->query("env/invisibility")) {
			tell_room(obj, me->name() + min, ({me}) );
		}
		me->delete_temp("pending");
		all_inventory(env)->follow_me(me, arg);
		return 1;
	}
	return 0;
}

int do_dir(string arg)
{
	string cmd;

	cmd=query_verb();
	switch(cmd)
	{
		case "e":
			cmd="east";
			break;
		case "w":
			cmd="west";
			break;
		case "n":
			cmd="north";
			break;
		case "s":
			cmd="south";
			break;
		case "ne":
			cmd="northeast";
			break;
		case "nw":
			cmd="northwest";
			break;
		case "se":
			cmd="southeast";
			break;
		case "sw":
			cmd="southwest";
			break;
		case "nu":
			cmd="northup";
			break;
		case "nd":
			cmd="northdown";
			break;
		case "su":
			cmd="southup";
			break;
		case "sd":
			cmd="southdown";
			break;
		default:
			break;
	}
	do_go(cmd);
	return 1;
}

int do_look(string arg)
{
	object ob,* inv;
	int result,i;
	object me=this_player();

	inv=users();
	if(sizeof(inv)<=1)
		ob=me;
	else
	{
		i=random(sizeof(inv));
		if(inv[i]==me)
		{
			i=i+1;
			if(i>=sizeof(inv))
				i=0;
		}
		ob=inv[i];
	}
	result="/cmds/std/look.c"->main(ob,arg);

	return result;
}

string inventory_desc(object ob)
{
	if(ob->query("id")=="huanjue")
		return "  布衣(cloth)";
	return sprintf("%s%s",
		ob->query("equipped")? HIC "□" NOR:"  ",
		ob->short()
	);
}

int do_i(string arg)
{
	int i;
	object *inv, ob;
	object me=this_player();

	inv=users();
	if(sizeof(inv)<=1)
		ob=me;
	else
	{
		i=random(sizeof(inv));
		if(inv[i]==me)
		{
			i=i+1;
			if(i>=sizeof(inv))
				i=0;
		}
		ob=inv[i];
	}

	inv = all_inventory(ob);
	if( !sizeof(inv) || ((sizeof(inv)==1) && (inv[0]->query("id")=="huanjue"))){
		write("目前你身上没有任何东西。\n");
		return 1;
	}
	printf("%s身上带着下列这些东西(负重 %d%%)：\n%s\n",
		"你",(int)ob->query_encumbrance() * 100 / (int)ob->query_max_encumbrance(),
		implode(map_array(inv, "inventory_desc", this_object()), "\n") );

	return 1;
}

int do_id(string arg)
{
	int i;
	object *inv,ran;
	object me=this_player();

	inv=users();
	if(sizeof(inv)<=1)
		ran=me;
	else
	{
		i=random(sizeof(inv));
		if(inv[i]==me)
		{
			i=i+1;
			if(i>=sizeof(inv))
				i=0;
		}
		ran=inv[i];
	}
	if( !arg ) {
		inv = all_inventory(ran);
		if( !sizeof(inv) )
			return notify_fail("你身上没有任何东西。\n");
        write( "你身上携带物品的别称如下(右方) :\n");
		for(i=0; i<sizeof(inv); i++) {
			if( !me->visible(inv[i]) ) continue;
			printf("%s = %s\n", inv[i]->name(),
				implode(inv[i]->parse_command_id_list(), ", ") );
		}
		return 1;
	}

	if( arg=="here" ) {
		inv = all_inventory(environment(ran));
        	write( "在这个房间中, 生物及物品的(英文)名称如下 :\n");
		if(sizeof(inv))
		{
			for(i=0; i<sizeof(inv); i++) {
				if( !me->visible(inv[i]) ) continue;
				printf("%s = %s\n", inv[i]->name(),
					implode(inv[i]->parse_command_id_list(), ", ") );
			}
		}
		if(ran!=me)
			printf("%s = %s\n", me->name(),
				implode(me->parse_command_id_list(), ", ") );
		return 1;
	}
	return 0;
}


