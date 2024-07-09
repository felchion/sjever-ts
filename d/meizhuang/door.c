// Room: /d/meizhuang/entrance.c 庄院大门
// By Jpei, recode by Lklv
// Modify and open Quest by Lklv 2001.9.11

#include <ansi.h>
inherit ROOM;

void create()
{
	set("short", RED"庄院大门"NOR);
	set("long",
"眼前是高高的白石墙，石墙围绕着的是一座气派的庄院。大门上高高挂着\n"
"块"CYN"匾额（bian）"NOR"，匾上写着"HIY"【梅庄】"NOR"两个烫金大字，字写得笔力苍劲，儒雅之\n"
"中透着勃勃英气。前面是两扇朱红色的大门（gate），门上的大铜环擦得精光\n"
"雪亮。\n"
	);
	set("outdoors","杭州");
	set("flag", 0);
	set("item_desc", ([
		"gate" : "大门紧闭着，门上有个擦得精光雪亮的大铜环，看来是敲(qiao)门用的。\n",
		"door" : "大门紧闭着，门上有个擦得精光雪亮的大铜环，看来是敲(qiao)门用的。\n",
		"bian" : "匾上写着“梅庄”两个大字，旁边还署着四个小字：“虞允文题”。\n",
	]) );
	set("exits", ([
		"north" : __DIR__"road3",
	]));
	setup();
}

void init()
{
	object me = this_player();

	add_action("do_knock", ({"qiao"}));
	if (!me->query_temp("mz_quest")){
		add_action("do_answer", ({"answer", "huida", "da"}));
		add_action("do_show", ({"show", "zhanshi", "chushi", "shi"}));
	}
}

int do_knock(string arg)
{
	object me, env;
	string times, gate;
	int count, knock;

	me = this_player();
	env = this_object();
	knock = me->query_temp("mz_quest/knock");

	if (!arg) return notify_fail("你要敲什么？\n");
	if (knock == 4 && !me->query_temp("mz_quest/flag"))
		return notify_fail("门里有个声音喊道：“等等，这就来了！”\n");
	if (me->is_busy()&& !wizardp(me))
		return notify_fail("慢点，你上一个动作还没有完成。\n");
	if (sscanf(arg, "%s %d %s", gate, count, times) != 3 || gate != "gate" || times != "times")
		return notify_fail("你要敲几次( ? times)？\n");
	if ( count > 10 ) return notify_fail("嘘，轻点！\n");
	if (present("ding jian", env) && present("shi lingwei", env))
		return notify_fail("两个家人喝道：“人都在这了，还敲什么？！”\n");
	if (present("ding jian", env) && !present("shi lingwei", env)){
		new(__DIR__"npc/shilingwei")->move(env);
		return notify_fail("一个家人走出来,看了几眼,生气地说：“这里不是有人吗，还敲什么？！”\n");
	}
	if (present("shi lingwei", env) && !present("ding jian", env)) {
		new(__DIR__"npc/dingjian")->move(env);
		return notify_fail("一个家人走出来,看了几眼,生气地说：“这里不是有人吗，还敲什么？！”\n");
	}

	message_vision(HIW"$N抓住门上的大铜环，敲了" + CHINESE_D->chinese_number(count) + "下。\n"NOR, me);
	me->start_busy(1);

	if ((knock == 0 && count == 4) || (knock == 1 && count == 2) ||	(knock == 2 && count == 5))
		me->add_temp("mz_quest/knock", 1);
	else {
		if (knock == 3 && count == 3){
			me->set_temp("mz_quest/knock", 4);
			call_out("servant_out", 30, me);
			}
		else{
			remove_call_out("servant_out");
			me->delete_temp("mz_quest/knock");
		}
	}
	return 1;
}

void servant_out(object me)
{
	object shi, ding, env = this_object();

	if (!objectp(me)) return;
	if (!(shi = present("shi lingwei", env))) {
		shi = new(__DIR__"npc/shilingwei");
		shi->move(env);
	}
	if (!(ding = present("ding jian", env))) {
		ding = new(__DIR__"npc/dingjian");
		ding->move(env);
	}
	tell_room(env, CYN"过了半晌，大门缓缓打开，并肩走出两个家人装束的老者。\n"NOR, ({}));
	if (!present(me, env)) {
		tell_room(env, CYN"两人看了半天，没找到敲门之人，于是就转身走了回去，把大门关上了。\n"NOR, ({}));
		destruct(shi);
		destruct(ding);
		return;
	}
	message_vision(CYN"左首的那人对着$N躬身说道：“这位" + RANK_D->query_respect(me) + "驾临敝庄，不知有何贵干？”\n"NOR, me);
	me->set_temp("mz_quest/flag", 1);
	call_out("close_door", 180, me);
}

void close_door(object me)
{
	object shi, ding, env = this_object();

	if (shi = present("shi lingwei", env)) destruct(shi);
	if (ding = present("ding jian", env))  destruct(ding);
	if (query("flag")!=1){
		tell_room(env, CYN"两个家人等了半天，没有得到明确的回答(answer)，于是就转身走了回去，把大门关上了。\n"NOR, ({}));
	}
	else {
		tell_room(env, CYN"大门嘎吱一声响，从里面关上了。\n"NOR, ({}));
	}
	if (objectp(me)) me->delete_temp("mz_quest");
	delete("exits/south");
	return;
}

int do_answer(string arg)
{
	object me = this_player();
	object env = this_object();

	if (!me->query_temp("mz_quest") || me->query_temp("mz_quest/flag") != 1) return 0;
	if (!arg) return notify_fail("你要回答什么？\n");

	if (!present("shi lingwei", env))
		new(__DIR__"npc/shilingwei")->move(env);
	if (!present("ding jian", env))
		new(__DIR__"npc/dingjian")->move(env);
	if (arg[0..5] == "求见江南四友" || arg[0..5] == "拜见江南四友"
	 ||arg[0..7] == "有事求见江南四友" || arg[0..7] == "有事拜见江南四友") {
		message_vision(CYN"$N拱手道：“在下$P有事求见江南四友四位前辈，劳烦二位引见。”\n"NOR,me);
		message_vision(CYN"左首的家人对$N道：“我家主人向不见客。”说着便欲关门。\n"NOR, me);
		me->set_temp("mz_quest/flag", 2);
		return 1;
	}
	else{
		message_vision(CYN"$N拱手道：“在下路过此地，想借宿一宿，劳烦二位行个方便。”\n"NOR,me);
		message_vision(CYN"右首的家人对$N道：“本庄一向不留外客，除非你是我家主人的朋友。”说着就要关门。\n"NOR, me);
	}
	return 1;
}

int do_show(string arg)
{
	object me, env, *inv, shi, ding;
	string str;
	int i, flag;

	me = this_player();
	env = this_object();
//	ob = present("wuyue lingqi", me);

	if (!me->query_temp("mz_quest/flag") || !me->query_temp("mz_quest") )
	return 0;
	if (!arg) return notify_fail("你要展示什么？\n");
	inv = all_inventory(me);
	for (i = 0; i < sizeof(inv); i++)
		if (inv[i]->id(arg)) break;
	if (i == sizeof(inv)) return notify_fail("你要展示什么？\n");
	if (!(shi = present("shi lingwei", env))) {
		shi = new(__DIR__"npc/shilingwei");
		shi->move(env);
	}
	if (!(ding = present("ding jian", env))) {
		ding = new(__DIR__"npc/dingjian");
		ding->move(env);
	}
	if (!me->query_temp("mz_quest/flag"))
		return notify_fail("你掏出一" + inv[i]->query("unit") + inv[i]->query("name") + "，想给大家看，不过没人理你。\n");
	flag = 0;
	str = "$N掏出一" + inv[i]->query("unit") + inv[i]->query("name") + "，";
	if (inv[i]->id("wuyue lingqi")) {
		flag = 1;
		str += "展了开来。只见宝光四耀，上面镶满了珍珠宝石。\n";
	}
	else str += "对着两个家人晃了两晃。不过他们好象不明白$N的意思！\n";
	message_vision(str, me);
	if (flag) {
		if (me->query_temp("mz_quest/flag") == 1) {
			message_vision(CYN"两个家人对$N喝道：“怎么，想拿左冷禅的烂旗子来吓唬我们？先回答(answer)来干什么的再说！\n"NOR, me);
			return 1;
		}
		else {
			message_vision(HIW"两个家人齐声道：“嵩山派左盟主的令旗？”\n$N道：“正是。”\n"NOR, me);
			message_vision(HIW"右首的家人道：“即使是嵩山左盟主亲到，我家主人也未必...”\n"NOR, me);
			message_vision(HIW"左首的家人对$N道：“不过既然有令旗在，你且进去试试运气。”\n"NOR, me);
			message_vision(CYN"家人让在两旁，对$N一拱手道：“里边请。”\n"NOR, me);
			set("exits/south", __DIR__"tianjing");
			me->force_me("go south");
			ding->force_me("go south");
			shi->force_me("go south");
			me->set_temp("mz_quest/flag", 3);
			remove_call_out("close_door");
			call_out("close_door", 60, me);
//			destruct(inv[i]);
		}
	}
	return 1;
}
int valid_leave(object me, string dir)
{
	if (dir == "north" && me->query_temp("mz_quest/flag")==3
	&& (objectp(present("ding jian", environment(me)))
	|| objectp(present("shi lingwei", environment(me))) )){
		return notify_fail(CYN"两个家人拦在你的身前道：“" + RANK_D->query_respect(me) + "留步，请随我二人进庄罢。”\n"NOR);
	}
	if (me->query_temp("mz_quest/flag")==3)
		me->set_temp("mz_quest/go",1);
	if (dir == "south" && me->query_temp("mz_quest/flag")!=3)
		return notify_fail(CYN"两个家人拦在你的身前道：“" + RANK_D->query_respect(me) + "，我家庄主不见外客。”\n"NOR);
        return ::valid_leave(me, dir);
}