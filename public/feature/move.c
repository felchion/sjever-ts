// move.c
// Modified by snowman@SJ 21/11/2000
// 屏蔽 old_ob
// Modified again by snowman@SJ 13/02/2001
// Modified by augx@SJ 9/19/2001

//#pragma optimize

#include <ansi.h>
#include <dbase.h>
#include <mudlib.h>
#include <room.h>

static int weight = 0;
static int encumb = 0, max_encumb = 0;

nomask int query_encumbrance() { return encumb; }
nomask int over_encumbranced() { return encumb > max_encumb; }

nomask int query_max_encumbrance() { return max_encumb; }
nomask void set_max_encumbrance(int e) { max_encumb = e; }
nomask void add_encumbrance(int w)
{
	object me = this_object();

	encumb += w;
	if (encumb < 0 && !wizardp(me)) log_file("move.bug", sprintf("%O encumbrance underflow.\n", me));
	if (encumb > max_encumb) me->over_encumbrance();
	if (environment()) environment()->add_encumbrance(w);
}

void over_encumbrance()
{
	object me = this_object();

	if (!interactive(me)) return;
	tell_object(me, "你的负荷过重了！\n");
}

nomask int query_weight() { return weight; }
nomask void set_weight(int w)
{
	if (!environment()) {
		weight = w;
		return;
	}
	if (w!=weight) environment()->add_encumbrance(w - weight);
	weight = w;
}


// This is the "current" weight of an object, which is used on weight
// checking in move().
nomask int weight() { return weight + encumb; }

varargs int move(mixed dest, int silently)
{
	object me = this_object();
	object where = environment();
	object ob, env;
	object *inv;
	mapping exits;
	string *dirs;
	string str, arg;
	int t;
	int i;
	int * dirs2d = ({ 0,0,0,0,0,0,0,0,0,0,0 });

	// If we are equipped, unequip first.
	if (where && query("equipped") && !me->unequip())
			return notify_fail("你没有办法取下这样东西。\n");

	// Find the destination ob for moving.
	if (objectp(dest))
		ob = dest;
	else if (stringp(dest)) {
		ob = load_object(dest);
		if (!ob)
			error(sprintf("%O %O -> %O\n", me, where, dest));
	} else
		error(sprintf("%O %O -> %O\n", me, where, dest));

	if(!IN_MAIN && CP_D->invalid_move(me,ob)) 
	return notify_fail("这个方向无法移动。\n");
//	
	// Check if the destination ob can hold this object.
	// Beforce checking it, we check if the destination is environment of
	// this_object() (or environment of its environment). If it is, then
	// this could be like get something from a bag carried by the player.
	// Since the player can carry the bag, we assume he can carry the this
	// object in the bag and encumbrance checking is unessessary.
	env = me;
	while(env = environment(env)) if (env==ob) break;
	if ((ob==this_player() || userp(ob) || !living(ob)) && where && !env
	&& (int)ob->query_encumbrance() + weight + encumb > (int)ob->query_max_encumbrance()) {
		if (ob==this_player())
			return notify_fail( me->name() + "对你而言太重了。\n");
		else
			return notify_fail( me->name() + "对" + ob->name() + "而言太重了。\n");
	}

	// Move the object and update encumbrance
// Yuj 19991111
	if (where) {
		where->add_encumbrance( - weight - encumb);
		set("lastroom", base_name(where));
		if (!query_temp("unconcious") && !living(me)) set_temp("unconcious", where);
	}
	ob->add_encumbrance(weight + encumb);  // Yuj 20010508
// Yuj 19991111
	move_object(ob);
	if (!me) {
		message("wizard:yuj", sprintf("error: %O\n", ob), users());
		log_file("move", sprintf("error: %O\n", ob));
		return 0;
	}
	env = environment();
	//ciwei@SJ update here
	if (where && query("unique") && !me->is_character() && !query("embed")
	&& !mapp(query("skill")) && userp(where) && !wizardp(where) && where != env) {
		if (!query("no_get"))
			set("no_get", 1);
	}
	// If we are players, try look where we are.
	if (interactive(me)	// are we linkdead?
	&& living(me)		// are we still concious?
	&& !silently) {
		t = time();

		if (query("env/brief") || query_temp("pending/follow") || t - query_temp("last_go_time") < 2) {
			str = env->query("short");
			if (!stringp(str)) str = "";
			
			// Client Send
			if( me && userp(me) && CLIENT_D->IsClient(me) ) {
				CLIENT_D->SendRoomLong(me,str);
				str = "";
			}
			// Client Send
			
			if (mapp(exits = env->query("exits")) ) {
				dirs = sort_array(keys(exits), 1);
				for(i=0; i<sizeof(dirs); i++)
					if ((int)env->query_door(dirs[i], "status") & DOOR_CLOSED )
						dirs[i] = 0;
				dirs -= ({ 0 });
				if( !query("env/newlook") || query_temp("pending/follow") ) {
					if (sizeof(dirs)) str += " - " + BOLD + implode(dirs, "、") + NOR;
				} else {
					for(i=0; i<sizeof(dirs); i++) {
						switch (dirs[i]) {
							case "east"      : dirs2d[0]=1; break;
							case "southeast" : dirs2d[1]=1; break;
							case "south"     : dirs2d[2]=1; break;
							case "southwest" : dirs2d[3]=1; break;
							case "west"      : dirs2d[4]=1; break;
							case "northwest" : dirs2d[5]=1; break;
							case "north"     : dirs2d[6]=1; break;
							case "northeast" : dirs2d[7]=1; break;
							case "up"        : dirs2d[8]=1; break;
							case "down"      : dirs2d[9]=1; break;
							case "enter" 	 : dirs2d[10]=1; break;
							default: continue;
						}
						dirs -= ({ dirs[i] });
						i--;
					}

					str += "\n";
					if(dirs2d[8])  str+=HIR+"   up  \n"NOR;
					if(dirs2d[5])  str+=HIR; str += "nw "NOR;
					if(dirs2d[6])  str+=HIR; str += "n "NOR;
					if(dirs2d[7])  str+=HIR; str += "ne\n"NOR;
					if(dirs2d[4])  str+=HIR; str += "w  "NOR;
					if(dirs2d[10]) str+=HIR; str += "r "NOR;
					if(dirs2d[0])  str+=HIR; str += " e"NOR;
					if (sizeof(dirs)) str += " - "+BOLD+implode(dirs, "、")+NOR; str+="\n";
					if(dirs2d[3])  str+=HIR; str += "sw "NOR;
					if(dirs2d[2])  str+=HIR; str += "s "NOR;
					if(dirs2d[1])  str+=HIR; str += "se"NOR;
					if(dirs2d[9])  str+=HIR"\n  down  "NOR;
				}
			}

			str += "\n";
			exits = ([]);
			if (!query_temp("pending/follow")) {
				inv = all_inventory(env);
				for(i=0; i<sizeof(inv); i++) {
					if (inv[i]==me || !me->visible(inv[i]) ) continue;
					if (userp(inv[i]) ){
						str += "  " + inv[i]->short() + "\n";
						continue;
					}
					arg = base_name(inv[i]) + inv[i]->short();
					if (!mapp(exits[arg]) ) {
						exits[arg] = ([
							"total" : 1,
							"unit" : (inv[i]->query("unit")?inv[i]->query("unit"):"个"),
							"short" : "  "+inv[i]->short(),
						]);
					}
					else{
					  exits[arg]["short"] = inv[i]->short();
					  exits[arg]["total"]++;
					} 
				}
			} else {
				inv = all_inventory(env);
				if (sizeof(inv) > 1 && inv[1]!=me && me->visible(inv[1]))
					str += inv[1]->short() + "\n";
			}
			dirs = keys(exits);
			if (!sizeof(dirs)) {
				tell_object(me,str);
				// return 1;
			} else {
				for( i = 0; i < sizeof(dirs); i++ ){
					if (exits[dirs[i]]["total"] > 1 )
						arg = "  " + chinese_number(exits[dirs[i]]["total"]) + exits[dirs[i]]["unit"] + exits[dirs[i]]["short"];
					else 	arg = exits[dirs[i]]["short"];
					str += arg + "\n";
				}
				tell_object(me, str);
			}
		}
		else command("look");
		set_temp("last_go_time", t);
	}

	if (!silently && query("treasure") && userp(ob) && ob == env ) {
		str = ob->query("name");
		if (query("treasure") > 0) {
			str += "弄到了一"+ query("unit") + query("name");
			set("treasure", -1);
			set("old_ob", ob);
			CHANNEL_D->do_channel(this_object(), "rumor", str+"！");
		}
		else if (query("old_ob") != ob && random(15) == 7 ) {
			// 屏蔽，因为updated 检查宝物，看是否是真正的主人。
			//set("old_ob", ob);
			if (where->query("short"))
				str += "在" + where->query("short") +"捡到了一" + query("unit") + query("name");
			else
				str += "从" + where->query("name") +"处得到了一" + query("unit") + query("name");
			CHANNEL_D->do_channel(this_object(), "rumor", str+"。");
		}
	}
	env->init2(me, silently);
	
	// Client operate
	if( where && userp(where) && CLIENT_D->IsClient(where) )
		CLIENT_D->ChangeInv(where,me,0);
	if( ob && userp(ob) && CLIENT_D->IsClient(ob) )
		CLIENT_D->ChangeInv(ob,me,1);
	if( me && ob && userp(me) && CLIENT_D->IsClient(me) )
		CLIENT_D->ChangeRoom(me,ob);
	if( where && !clonep(where) )
		CLIENT_D->ChangeRoomObj(where,me,0);
	if( ob && !clonep(ob) )
		CLIENT_D->ChangeRoomObj(ob,me,1);
	// Client operate
	if(userp(me)) TASK_D->place_event(me,env);
	return 1;
}

void remove(string euid)
{
	object me = this_object();
	object default_ob;

	if (!previous_object()
	|| base_name(previous_object()) != SIMUL_EFUN_OB)
		error("move: remove() can only be called by destruct() simul efun.\n");

	if (userp(me)) {
		//mixed mysql;

		if (euid!=ROOT_UID && euid != geteuid(me)) {
			log_file("destruct", sprintf("%s attempt to destruct user object %s (%s)\n",
				euid, query("id"), ctime(time()))
			);
			error("你(" + euid + ")不能摧毁其他的使用者。\n");
		}
	} 
	else	me->unequip();

	// We only care about our own weight here, since remove() is called once
	// on each destruct(), so our inventory (encumbrance) will be counted as
	// well.
	if(!me)	return;
	
	if (environment())
		environment()->add_encumbrance(-weight-encumb);
	if (default_ob = me->query_default_object())
		default_ob->add("no_clean_up", -1);
	if(userp(me)){
		delete("ONLLINE");
		me->save();
	}
}

int move_or_destruct( object dest )
{
	if (userp(this_object())) {
		tell_object(this_object(), "一阵时空的扭曲将你传送到另一个地方....\n");
		move(VOID_OB);
	}
}
