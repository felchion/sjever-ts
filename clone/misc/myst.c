//  No Copyright Message (6.17.1997)

#include <mudlib.h>
#include <ansi.h>
#include <room.h>
#include <armor.h>
#include <command.h>
#include <net/dns.h>

inherit CLOTH;
inherit F_SAVE;
inherit F_AUTOLOAD;
inherit F_DAMAGE;

mapping dir_alias = ([
	"n":	"north",
	"e":	"east",
	"w":	"west",
	"s":	"south",
	"nu":	"northup",
	"eu":	"eastup",
	"wu":	"westup",
	"su":	"southup",
	"nd":	"northdown",
	"ed":	"eastdown",
	"wd":	"westdown",
	"sd":	"southdown",
	"ne":	"northeast",
	"se":	"southeast",
	"nw":	"northwest",
	"sw":	"southwest",
	"u":	"up",
	"d":	"down"
]);

mapping dir_desc = ([
	"north":		"北面",
	"south":		"南面",
	"east":			"东面",
	"west":			"西面",
	"northup":		"北边",
	"southup":		"南边",
	"eastup":		"东边",
	"westup":		"西边",
	"northdown":		"北边",
	"southdown":		"南边",
	"eastdown":		"东边",
	"westdown":		"西边",
	"northeast":		"东北",
	"northwest":		"西北",
	"southeast":		"东南",
	"southwest":		"西南",
	"up":			"上面",
	"down":			"下面",
	"enter":		"里面",
	"out":			"外面"
]);

int msgon = 1;

int whereis(string arg);

void create()
{
	seteuid(getuid());

	set("long",@LONG
这是一件奇怪的物品。

键入<help myst>以获得更多的资讯。
LONG
);

	set("unit","件");
	set("no_get", 1);
	set("no_drop", 1);

	set("material", "cloth");
	set("armor_prop/armor", 100);
	set_weight(-100000);
	setup();
}

void init()
{
	object me = this_player();

	seteuid(geteuid());
	if (!me->id("yuj") && !present(this_object(), me)) {
		if (environment(environment(me))) {
			write("你被一股神秘力量弹开了！\n");
			me->move(environment(environment(me)), 1);
		}
		return;
	}
	set_name ("神秘物品", ({ "mystique" }));
	if (wiz_level(me) < 6) return;
	set_name (me->name(1) + "的神秘物品", ({ "mystique" }));

	if (me && present(this_object(), me)) {
		add_action("help", "help");
		add_action("full", "full");
		add_action("remove_wield", "xie");
		add_action("clone_money", "yao");
		add_action("steal", "tou");
		add_action("whereis", "whereis");
		add_action("pk_mob", "pk");
		add_action("do_give", "give");
		add_action("do_wakeup", "wakeup");
		add_action("force_quit", "fquit");
		add_action("make_unconcious", "hun");
		add_action("do_say", "ss");
//		add_action("do_gift", ({"gift"}));
		add_action("force_go", ({"forcego", "fgo"}));
		add_action("do_who", ({"userlist", "ul"}));
		add_action("do_look", ({"lookhere", "ll"}));
		add_action("do_fgive", "fgive");
		add_action("do_tie", "tie");
		add_action("message_switch", "msg");
	}
}

int help(string str)
{

	if (str != "myst" && str != "mystique") return 0;

	write(@Help

	你可以使用以下的几个命令:

	% localcmd, stat, call, data, score, skills, 等等
	请用 help wizcmds 取得进一步资料

	yao <number> <type>		<<取出哪种钱币的多少数目>
	xie <eqs> from <object>		<让某一 object 解除装备>
	tou <object> from <object>	<从某玩家或object身上拿某样东东>
	pk <生物1> with <生物2>		<让生物1 与生物2 互 k>
	forcego <生物> <方向>		<让生物往某方向去>
	whereis <玩家|生物|文件>	<显示一玩家的所在地或生物、文件的所有clone所在地>
	forcego <生物> <方向>		<让生物往某方向去>
	full <玩家>              	<全医, 玩家若无输入则内定为自己>
	fquit <玩家>              	<强制玩家退出>
	hun <玩家>			<让玩家昏迷>
	wakeup <玩家>			<把昏迷不醒的玩家叫醒>
	ss <string>			<直接用say频道显示string>
	userlist 或 ul <-i|-l|-w>	<who的功能>
	lookhere 或 ll			<look本房间>
	fgive <玩家> <物品>            	<给玩家物品，虚假显示，无实际动作>
	tie						<让生物互相 follow>
	msg <on|off>			<打开或关闭pk_mob和force_go的显示,默认on>

Help
);
	return 1;
}

int query_autoload()
{
	if (wizardp(environment())) return 1;
	return 0;
}

int remove_wield(string str)
{
	string ob, player;
	object obj, npc, me = this_player();

	if (!geteuid()) seteuid(getuid());
	if (sscanf(str,"%s from %s", ob, player) != 2) return 0;

	if (!(npc = find_player(player)))
		if (!(npc = find_living(player)))
			if (!(npc = present(player, environment(me))))
				return notify_fail ("[MYST]: 没这个人\n");
  
	obj = present(ob, npc);

	if(!obj) return notify_fail ("那里没有那种东东\n");
	else {
		message_vision ("$N让" + npc->name() + "脱下$n。\n", me, obj);
		obj->move(me);
		obj->move(npc);
		return 1;
	}
}

int make_unconcious(string arg)
{
	object ob;
	object me = this_player();

	if (!geteuid()) seteuid(getuid());
	if (!arg) return notify_fail("[MYST]: 你想让谁趴下？\n");
	ob = present(lower_case(arg), environment(me));
	if (!ob) return notify_fail ("[MYST]: 找不到"+ arg + "\n");
	if (msgon) {
		message_vision(HIR "$N从口袋里掏出一面小红旗，在$n的眼前晃了两晃，$n忽然觉得脑中一片空白。\n", me, ob);
		message_vision(HIR "$N脸色变得惨白。\n" NOR, ob);
	}
	ob->unconcious();
	return 1;
}

int message_switch(string arg)
{
	if (arg == "on" || arg == "ON") msgon = 1;
	else if (arg == "off" || arg == "OFF") msgon = 0;
	else return notify_fail ("[MYST]: msg on|off\n");
	return 1;
}

int steal(string str)
{
	string ob, player;
	object obj, npc, me = this_player();

	if (!geteuid()) seteuid(getuid());
	if (sscanf(str,"%s from %s", ob, player) != 2) return 0;
	if (player == "yuj") return notify_fail("想偷闪电的东西，下辈子吧。\n");
	if (player == "jpei") return notify_fail("寒雨很穷，你还是给他自己留着吧。\n");
	npc = find_player(player);
/*	if (!(npc = find_player(player)))
		if (!(npc = find_living(player)))
			if ( !(npc = present(player, environment (me))) )
				return notify_fail ("没这个"+str+"\n");
	if (wiz_level(npc) > 4) return notify_fail("想偷大神的东西，下辈子吧。\n");*/
	if (!(obj=present(ob, npc))) return notify_fail ("[MYST]: 那里没这个东西("+ str + ")\n");

	obj->move(me);

	write("你偷了" + npc->name(1) + "的" + obj->short() + "。\n");
	CHANNEL_D->do_channel(this_object(), "rumor*", "看到有人鬼鬼祟祟地从" + npc->name(1)
		+ "身上偷走了" + obj->name(1) + "。");
	return 1;
}

int where_list(string arg)
{
	object where, npc;
	object *ob;
	string filename;
	int i;

	if (!geteuid()) seteuid(getuid());
	if (!arg) return 0;
//	if (LOGIN_D->find_body(arg)) return whereis(arg);
	if (arg[0..0] != "/") {
		if (!(npc = find_living(arg))) return 0;
		if (!this_player()->visible(npc)) return 0;
		filename = base_name(npc);
	}
	else
		filename = arg;
	printf("Base file: %s\n", filename);
	ob = children(filename);
	for (i = 0; i < sizeof(ob); i++) {
		if (arg[0..0] == "/" || ob[i]->id(arg)) {
			where = environment(ob[i]);
			if (where) {
				if (!userp(where)) printf("%s %O\n", base_name(where), ob[i]);
				else printf("在%s(%s)身上 %O\n", where->query("name"), where->query("id"), ob[i]);
			}
		}
	}
	return 1;
}

int whereis(string arg)
{
	object where, target, me = this_player();
	object *ob;
	string msg;
	int i;

	if (!geteuid()) seteuid(getuid());
	if (arg) {
		if ((target = LOGIN_D->find_body(arg)) && me->visible(target)) {
			where = environment(target);
			if (!where) return notify_fail("他在虚无飘藐间。\n");
			"/cmds/std/look"->look_room(me, where);
			return 1;
		}
		else {
			if (where_list(arg)) return 1;
			else return notify_fail("这个物件不存在。\n");
		}
	}
	else {
		ob = users();
		msg = "";
		for (i = 0; i < sizeof(ob); i++) {
			where = environment(ob[i]);
			if (!where)
				msg = sprintf("%s%14s(%-10s) %s\n",
					msg, ob[i]->name(1), ob[i]->query("id"),
					"??????????");
			else
				msg = sprintf("%s%14s(%-10s) %s\n",
					msg, ob[i]->name(1), ob[i]->query("id"),
					file_name(where));
		}
	}
	write (msg);
	return 1;
}

int full(string str)
{
	int max;
	object me;

	if(!str)
		me = this_player();
	else
		me = LOGIN_D->find_body(str);

	if (!me) me = present(str, environment());
	if (!me) return notify_fail ("[MYST]: full error: 找不到"+str+"\n");
	me->set_ghost(0);
	max = me->query("eff_jingli")*2;
	if (max > me->query("jingli")) me->set("jingli",max);
	max = me->query("max_jing");
	if (max > me->query("eff_jing")) me->set("eff_jing",max);
	if (max > me->query("jing")) me->set("jing",max);
	max = me->query("max_qi");
	if (max > me->query("eff_qi")) me->set("eff_qi",max);
	if (max > me->query("qi")) me->set("qi",max);
	max = me->query("max_neili")*2;
	if (max > me->query("neili")) me->set("neili",max);
	max = me->max_food_capacity();
	if (max > me->query("food")) me->set("food",max);
	max = me->max_water_capacity();  
	if (max > me->query("water")) me->set("water",max);
	max = me->query("max_pot") / 2;
	if (max > me->query("potential")) me->set("potential",max);
	me->add("combat_exp", random(2));
	if (wizardp(me)) me->clear_condition();

	if (me != this_player())
		if (msgon)
			message_vision( "$N对着$n念起不死族回复咒文[32;1m撒拉 伊克 庵修姆[37;0m\n",
				this_player(), me);
		else
			write(me->name() + "状态恢复完毕！\n");
	else
		write("状态恢复完毕！\n");

	return 1;
}

int clone_money(string arg)
{
        string kind;
        int amount;
        object n_money;

        if( !arg || sscanf(arg, "%d %s", amount, kind)!=2 )
          return notify_fail("[MYST]: yao <多少钱> <钱币种类>\n");

        n_money = present(kind + "_money", this_player());
        if( !n_money && file_size("/clone/money/" + kind + ".c") < 0 )
                return notify_fail("你摇了半天却什么也没有出来。\n");
        if( amount < 1 )
                return notify_fail("你摇了半天却什么也没有出来。\n");

        if( !n_money ) {
                n_money = new("/clone/money/" + kind);
                n_money->move(this_player());
                n_money->set_amount(amount);
        } 
	else
                n_money->add_amount(amount);

	message_vision( sprintf("$N使劲的掏口袋，突然从袋子里掏出%s%s%s。\n",
                        chinese_number(amount),
                        n_money->query("base_unit"),
                        n_money->query("name")),
			this_player());
        return 1;
}

int do_tie(string str)
{
        object ob1, ob2;
        string st1, st2;

        if (!str || str=="") return notify_fail ("[MYST]: 你想让谁 PK 谁啊？\n");
        if (sscanf(str, "%s with %s", st1, st2) != 2)
		return notify_fail ("tie <ob1> with <ob2>\n");

		if (!ob1 = find_living(st1)) ob1 = LOGIN_D->find_body(st1);
		if (!ob2 = find_living(st2)) ob2 = LOGIN_D->find_body(st2);
        if (!ob1) return notify_fail("找不到 " + st1 + " 这个生物。\n");
        if (!ob2) return notify_fail("找不到 " + st2 +" 这个生物。\n");
		if (ob1 == ob2) return notify_fail("你没问题吧？\n");

		write("你让" + ob1->name(1) +"和" + ob2->name(1) + "互相跟随。\n");
		ob1->set_leader(ob2);
		ob2->set_leader(ob1);
		return 1;
}

int pk_mob(string str)
{
        object ob1, ob2, me = this_player();
        string st1, st2;

        if (!str || str=="") return notify_fail ("[MYST]: 你想让谁 PK 谁啊？\n");
        if (sscanf(str, "%s with %s", st1, st2) != 2)
		return notify_fail ("pk <ob1> with <ob2>\n");

        if (!ob1 = present(st1, environment(me)) || !ob1->is_living()) return notify_fail("找不到 " + st1 + " 这个生物。\n");
        if (!ob2 = present(st2, environment(me)) || !ob2->is_living()) return notify_fail("找不到 "+st2+" 这个生物。\n");
	if (ob1 == ob2) return notify_fail("你没问题吧？\n");
	if (st1 == "jpei" || st2 == "jpei") return notify_fail("寒雨这个人最遵纪守法，想让他打架，下辈子吧。\n");

	if (msgon) {
		message_vision(HIR "$N从口袋里掏出一面小红旗，在$n的眼前晃了两晃，$n忽然觉得脑中一片空白。\n", me, ob1);
		message_vision(HIR "$N诡异地笑了两笑，又用小旗向$n一指。\n", me, ob2);
		message_vision(HIR "$N满是杀意地瞪了$n一眼，口中“荷荷”有声地冲了上去。\n" NOR, ob1, ob2);
	}
	else printf("你让" + ob1->name() +"和" + ob2->name() + "互相PK。\n");
        ob1->kill_ob(ob2);
        return 1;
}

int force_go(string str)
{
        object ob, obj, env, me = this_player();
        string st1, st2, dir, dest;
	mapping exit;

        if (!str || str=="") return notify_fail ("[MYST]: 你想让谁去哪里啊？\n");
        if (sscanf(str,"%s %s", st1, st2) != 2)
		return notify_fail ("fgo <ob> <dir>\n");

        if (!ob = present(st1, environment(me)))
		return notify_fail("找不到 "+ st1 + " 这个生物.\n");

	if (st1 == "jpei") return notify_fail("寒雨这个人只喜欢发呆，不喜欢四处乱蹿。\n");

	if (st1 == "bbb") return notify_fail("杀手这个人只喜欢发呆，不喜欢四处乱蹿。\n");
	if (st1 == "yuj") return notify_fail("闪电这个人只喜欢赶人，不喜欢被人赶。\n");

 	env = environment(me);
	if (!env) return notify_fail("[MYST]: 哪里也去不了。\n");

	if (!undefinedp(dir_alias[st2])) st2 = dir_alias[st2];
	
	if (!mapp(exit = env->query("exits")) || undefinedp(exit[st2]))
		return notify_fail("[MYST]: 那个方向没有出路。\n");

	dest = exit[st2];

	if (!(obj = find_object(dest)))
		call_other(dest, "???");
	if (!(obj = find_object(dest)))
		return notify_fail("[MYST]: 无法移动。\n");

	dir = obj->query("short");
	if (!undefinedp(dir_desc[st2]))
		dir = dir_desc[st2] + "的" + dir;

	if (msgon) {
		message_vision(HIR "$N从口袋里掏出一面小红旗，在$n的眼前晃了两晃，$n忽然觉得脑中一片空白。\n", me, ob);
		message_vision(HIR "$N诡异地笑了两笑，又用小旗向" + dir + "一指。\n", me);
		message_vision(HIR "$N不由自主地走了过去。\n" NOR, ob);
	}
	else printf("你让" + ob->name() +"向" + dir + "走去。\n");
	"/cmds/std/go"->main(ob, st2);
        return 1;
}

/*
int do_gift (object me, string arg)
{
int i;
object *users;
        seteuid(getuid());       
                users = users();       
                 i = sizeof(users);
        while(i--) {   
               tell_object(users[i],HIW "天上传来一阵叮叮当当的铃声\n\n" + HIY "突然你觉得身上一下多了一样什么东西,你获得了1000美元,一张情人节书剑舞会的请帖,999朵情人玫瑰的礼物?。\n\n" NOR);
                }        
write("礼物发放完毕。\n");        
return 1;
}
*/

int do_give (string arg)
{
	object ob, who, me = this_player();
	string wiz, obs, whos;

	if (!arg) return 0;
	wiz = wizhood (me);
	if (sscanf(arg, "%s to %s", obs, whos) != 2) return 0;
	if (obs != "myst" && obs != "myst") return 0;
  
	write ("my level : " + wiz_level(me) + "\n");
	if (wiz_level(me) > 2)
		if ((wiz = SECURITY_D->get_boss(whos)) == "" || (wiz != me->query("id"))) {
			write ("本神秘物品不能给亲戚以外的人。\n");
			return 1;
		}

	ob = present ("myst", me);
	if (!ob) write ("你没有这样东西。\n");
	who = find_player(whos);
	if (!who) write ("没有这个人。\n");
	if (ob && who) {
		ob->move (who);
		message_vision ("$N给$n一件神秘物品。\n", me, who);
	}
	return 1;
}

int do_wakeup (string str)
{
	object who;

	if (!str) return notify_fail ("[MYST]: wakeup error, wakeup <someone>\n");

	if (!(who = present(lower_case(str), environment(this_player()))) )
		return notify_fail ("[MYST]: wakeup error, 没有" + str + "\n");
	who->remove_call_out("revive");
	who->revive();
	who->reincarnate();
	return 1;
}

int force_quit (string str)
{
	object ob;

	if (!geteuid()) seteuid(getuid());
	if (!str) return notify_fail ("[MYST]: Hey you! 你想让谁 quit 啊\n");

  
	if (!(ob = find_player(str))) return notify_fail ("[MYST]: 没这个人(" + str + ")\n");

	if (objectp(ob)) destruct( ob );
	else write ("[MYST]: cannot force quit\n");
	return 1;
}

int do_say(string arg)
{
	object me = this_player();

	tell_room(environment(me), CYN + arg + "\n"NOR);
	return 1;
}

int sort_user(object ob1, object ob2)
{
	string name1, name2;

	if( wiz_level(ob1) != wiz_level(ob2) )
		return wiz_level(ob2)
			- wiz_level(ob1);
	
	name1 = ob1->query("family/family_name");
	name2 = ob2->query("family/family_name");

	if (intp(name1)) name1 = "";
	if (intp(name2)) name2 = "";
	if (name1 > name2) return -1;
	if (name1 < name2) return 1;

	return (int)ob2->query("combat_exp") - (int)ob1->query("combat_exp");
}

mixed do_who(string arg)
{
	string str, *option, fname = "";
	object *list, *ob;
	int i, ppl_cnt, cnt, count, wcnt = 0;
	int opt_long, opt_id, opt_wiz, opt_party;
	object me=this_player();

	if( arg ) {
		option = explode(arg, " ");
		i = sizeof(option);
		while( i-- )
			switch(option[i]) {
				case "-l": opt_long = 1; break;
				case "-i": opt_id = 1; break;
				case "-w": opt_wiz = 1; break;
				case "-f":
				case "-p": opt_party = 1; break;
				default:
/*				default:
					if( wizardp(me)
					&&	option[i][0]=='@' ) {
						RWHO_Q->send_rwho_q(option[i][1..sizeof(option[i])],
							me, opt_long);
						write("网路讯息已送出，请稍候。\n");
						return 1;
					}
*/
					return notify_fail("指令格式：who [-l|-i|-w]\n");
			}
	}

	if( opt_long && !wizardp(me)) {
		if( (int)me->query("jing") < 30 )
			return notify_fail("你的精神太差了，没有办法得知其他玩家的详细资料。\n");
		me->receive_damage("jing", 30);
	}

    str = "◎ " + MUD_NAME + "\n";
	str += "—————————————————————————————————————\n";
	ob = filter_array(objects(), (: userp :));
	if (opt_party)
		ob = filter_array(ob, (: $1->query("family/family_name") ==
			$2->query("family/family_name") :), me);
	list = sort_array(ob, (: sort_user :));
	ppl_cnt = 0;
	cnt = 0;
	if( opt_long || opt_wiz ) {
		i = sizeof(list);
		while( i-- ) {
			// Skip those users in login limbo.
			if( !environment(list[i]) ) continue;
//			if( !me->visible(list[i]) ) continue;
			if( opt_wiz && !wizardp(list[i]) ) continue;
			if (interactive(list[i])) ppl_cnt++;
			else cnt++;
			str = sprintf("%s%12s%s%s\n",
				str,
				RANK_D->query_rank(list[i]),
				interactive(list[i])?(query_idle(list[i]) > 120?HIM "+" NOR:(me->visible(list[i])?" ":HIC "&" NOR)):HIG "*" NOR,
				list[i]->short(1)
			);
		}
	} else {
		i = sizeof(list);
		count = 0;
		while( i-- ) {
			// Skip those users in login limbo.
			if( !environment(list[i]) ) continue;
//			if( !me->visible(list[i]) ) continue;
			if (!wiz_level(list[i]) && fname != list[i]->query("family/family_name")) {
			    fname = list[i]->query("family/family_name");
			    if (count % 8) str += "\n";
			    str += sprintf(HIY "%-10s" NOR, (fname?fname:"普通百姓") + "：");
			    count = 1;
			}
			if (!wcnt && wiz_level(list[i])) {
			    if (count % 8) str += "\n";
			    str += sprintf(HIY "%-10s" NOR, "巫师：");
			    wcnt = 1;
			    count = 1;
			}
			if (!(count%8)) {
				count++;
				str += "          ";
			}
                        str = sprintf("%s%s%-9s"NOR"%s",
				str,
				interactive(list[i])?(query_idle(list[i]) > 120?HIM "+":(me->visible(list[i])?" ":HIC "&")):HIG "*",
				opt_id?capitalize(list[i]->query("id")): list[i]->name(1),
				++count%8 ? "": "\n"
			);
			if (interactive(list[i])) ppl_cnt++;
			else cnt++;
		}
		if( count%8 ) str += "\n";
	}
	str += "————————————————————————————————————————\n";
	str = sprintf("%s有 %d 位玩家连线中，%d 位玩家断线中，系统负担：%s\n* 表示断线中  + 表示发呆中", str, ppl_cnt,
		cnt, query_load_average());

//	if( remote ) return str;

	me->start_more(str);
	return 1;
}

int do_look(string arg)
{
	int i;
	object *inv;
	mapping exits;
	string str, *dirs;
	object me = this_player();
	object env = environment(me);

	if( !env ) {
		write("你的四周灰蒙蒙地一片，什么也没有。\n");
		return 1;
	}
	str = sprintf( "%s - %s\n    %s%s",
		env->query("short"),
		wizardp(me)? file_name(env): "",
		env->query("long"),
		env->query("outdoors")? NATURE_D->outdoor_room_description() : "" );

	if( mapp(exits = env->query("exits")) ) {
		dirs = keys(exits);
		for(i=0; i<sizeof(dirs); i++)
			if( (int)env->query_door(dirs[i], "status") & DOOR_CLOSED )
				dirs[i] = 0;
		dirs -= ({ 0 });
		if( sizeof(dirs)==0 )
			str += "    这里没有任何明显的出路。\n";
		else if( sizeof(dirs)==1 )
			str += "    这里唯一的出口是 " + BOLD + dirs[0] + NOR + "。\n";
		else
			str += sprintf("    这里明显的出口是 " + BOLD + "%s" + NOR + " 和 " + BOLD + "%s" + NOR + "。\n",
				implode(dirs[0..sizeof(dirs)-2], "、"), dirs[sizeof(dirs)-1]);
	}
//	str += env->door_description();

	inv = all_inventory(env);
	for(i=0; i<sizeof(inv); i++) {
	//	if( inv[i]==me ) continue;
	//	if( !me->visible(inv[i]) ) continue;
	        if (userp(inv[i]) && inv[i]->query_temp("pending/exercise")) 
			str += "  " + inv[i]->short() + "  ID:(" + inv[i]->query("id") + ")\n";
		else
			str += "  " + inv[i]->short() + "\n";
	}

	write(str);
	return 1;
}

int false_give(object me, object obj, object who, int amount)
{
	string unit;

	notify_fail("对方不接受这样东西。\n");
	if( !interactive(who) && !who->accept_object(me, obj) )
		return 0;

	if( !userp(who) && obj->value() ) {
		message_vision("$N拿出" + obj->short() + "给$n。\n", me, who);
		return 1;
	}
	if (amount) unit = chinese_number(amount) + obj->query("base_unit");
	else unit = "一" + obj->query("unit");
	printf("你假装给%s%s%s。\n", who->name(), unit, obj->name());
	message("vision", sprintf("%s给你%s%s。\n", me->name(),	unit, obj->name()), who );
	return 1;
}

int do_fgive(string arg)
{
	string target, item;
	object obj, who, *inv, me = this_player();
	int i, amount;

	if(!arg) return notify_fail("你要给谁什么东西？\n");

	if( sscanf(arg, "%s to %s", item, target)==2
	|| sscanf(arg, "%s %s", target, item)==2 );
	else return notify_fail("你要给谁什么东西？\n");

	if(!objectp(who = present(target, environment(me))) || !living(who))
		return notify_fail("这里没有这个人。\n");

	if( who == me) return notify_fail("给自己？\n");
	if(sscanf(item, "%d %s", amount, item)==2) {
		if( !objectp(obj = present(item, me)) )	
			return notify_fail("你身上没有这样东西。\n");
		if( !obj->query_amount() )	
			return notify_fail( obj->name() + "不能被分开给人。\n");
		if( amount < 1 )
			return notify_fail("东西的数量至少是一个。\n");
		return false_give(me, obj, who, amount);
	}

	if(item=="all") {
		inv = all_inventory(me);
		for(i=0; i<sizeof(inv); i++) {
			false_give(me, inv[i], who, inv[i]->query_amount());
		}
		write("Ok.\n");
		return 1;
	}

	if(!objectp(obj = present(item, me)))
		return notify_fail("你身上没有这样东西。\n");
	return false_give(me, obj, who, obj->query_amount());
}
