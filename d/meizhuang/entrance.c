// Room: /d/meizhuang/entrance.c 庄院大门
// By Jpei
// modify by Lklv 2001.9.11

inherit ROOM;

int do_knock(string arg);
int do_answer(string arg);
int do_show(string arg);

void create()
{
	set("short", "庄院大门");
	set("long", @LONG
行到近处，只见庄院大门(gate)外写着“梅庄”两个大字，旁边署着“虞
允文题”四字。这几个字儒雅之中透着勃勃生气，不愧是出自南宋破金的大功
臣之手。
LONG
	);
	set("outdoors","杭州");

	set("item_desc", ([
		"gate" : "大门紧闭着，门上有个擦得精光雪亮的大铜环，看来是敲(qiao)门用的。\n",
	]) );
	set("exits", ([
		"north" : __DIR__"road3",
	]));
	setup();
}

void init()
{
	object me = this_player();

	me->delete_temp("plummanor");
	add_action("do_knock", ({"qiao"}));
	add_action("do_answer", ({"answer", "huida", "da"}));
	add_action("do_show", ({"show", "zhanshi", "chushi", "shi"}));
}

int do_knock(string arg)
{
	object me = this_player();
	object env = this_object();
	string times, gate;
	int count, knockgate;

	if (me->is_busy()&& !wizardp(me)) return notify_fail("慢点，你上一个动作还没有完成。\n");
	if (!arg) return notify_fail("你要敲什么？\n");
	if (sscanf(arg, "%s %d %s", gate, count, times) != 3 || gate != "gate" || times != "times")
		return notify_fail("你要敲几次( ? times)？\n");
	if (present("ding jian", env) && present("shi lingwei", env))
		return notify_fail("两个家人喝道：“人都在这了，还敲什么？！”\n");
	else if (present("ding jian", env)) {
		new(__DIR__"npc/shilingwei")->move(env);
		return notify_fail("一个家人走出来,看了几眼,生气地说：“这里不是有人吗，还敲什么？！”\n");
	}
	else if (present("shi lingwei", env)) {
		new(__DIR__"npc/dingjian")->move(env);
		return notify_fail("一个家人走出来,看了几眼,生气地说：“这里不是有人吗，还敲什么？！”\n");
	}
	message_vision("$N抓住门上的大铜环，敲了" + CHINESE_D->chinese_number(count) + "下。\n", me);
	me->start_busy(1);
	knockgate = me->query_temp("plummanor/knock_gate");
	if ((knockgate == 0 && count == 4) || (knockgate == 1 && count == 2) ||	(knockgate == 2 && count == 5))
		me->add_temp("plummanor/knock_gate", 1);
	else {
		if (knockgate == 3 && count == 3)
			call_out("servant_out", 30, me);
		else
			remove_call_out("servant_out");
		me->delete_temp("plummanor/knock_gate");
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
	tell_room(env, "过了半晌，大门缓缓打开，并肩走出两个家人装束的老者。\n", ({}));
	if (!present(me, env)) {
		tell_room(env, "两人看了半天，没找到敲门之人，于是就转身走了回去，把大门关上了。\n", ({}));
		destruct(shi);
		destruct(ding);
		return;
	}
	message_vision("左首的那人对着$N躬身说道：“这位" + RANK_D->query_respect(me) + "驾临敝庄，不知有何贵干？”\n", me);
	me->set_temp("plummanor/flag", 1);
	call_out("close_door", 180, me);

}

void close_door(object me)
{
	object shi, ding, env = this_object();

	if (shi = present("shi lingwei", env)) destruct(shi);
	if (ding = present("ding jian", env))  destruct(ding);
	tell_room(env, "两个家人等了半天，没有得到明确的回答(answer)，于是就转身走了回去，把大门关上了。\n", ({}));
	if (objectp(me)) me->delete_temp("plummanor");
	return;
}

int do_answer(string arg)
{
	object me = this_player();
	object env = this_object();

	if (!arg) return notify_fail("你要回答什么？\n");
	if (me->query_temp("plummanor/flag") != 1)
		return notify_fail("又没人问你，跟着瞎掺和什么？！\n");
	if (arg[0..1] == "求见"|| arg[0..5] == "求见江南四友" || arg[0..5] == "拜见江南四友" ||
		arg[0..7] == "有事求见江南四友" || arg[0..7] == "有事拜见江南四友") {
		if (!present("shi lingwei", env))
			new(__DIR__"npc/shilingwei")->move(env);
		if (!present("ding jian", env))
			new(__DIR__"npc/dingjian")->move(env);
		message_vision("$N拱手道：“在下$P有事求见江南四友四位前辈，劳烦二位引见。”\n",me);
		message_vision("左首的家人对$N道：“我家主人向不见客。”说着便欲关门。\n", me);
		me->set_temp("plummanor/flag", 2);
		return 1;
	}
	else
	message_vision("$N拱手道：“在下路过此地，想借宿一宿，劳烦二位行个方便。”\n",me);
	message_vision("右首的家人对$N道：“本庄一向不留外客，除非你是我家主人的朋友。”说着就要关门。\n", me);
	return 1;
}

int do_show(string arg)
{
	object me, env, *inv, shi, ding, dest;
	string str;
	int i, flag;

	me = this_player();
	env = this_object();

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
	if (!me->query_temp("plummanor/flag"))
		return notify_fail("你掏出一" + inv[i]->query("unit") + inv[i]->query("name") + "，想给大家看，不过没人理你。\n");
	flag = 0;
	str = "$N掏出一" + inv[i]->query("unit") + inv[i]->query("name") + "，";
	if (inv[i]->id("wuse lingqi")) {
		flag = 1;
		str += "展了开来。只见宝光四耀，上面镶满了珍珠宝石。\n";
	}
	else str += "对着两个家人晃了两晃。不过他们好象不明白$N的意思！\n";
	message_vision(str, me);
	if (flag) {
		if (me->query_temp("plummanor/flag") == 1) {
			message_vision("两个家人对$N喝道：“怎么，想拿左冷禅的烂旗子来吓唬我们？先回答(answer)来干什么的再说！\n", me);
		}
		else {
			message_vision("两个家人齐声道：“嵩山派左盟主的令旗？”\n$N道：“正是。”\n", me);
			message_vision("右首的家人道：“即使是嵩山左盟主亲到，我家主人也未必...”\n", me);
			message_vision("左首的家人对$N道：“不过既然有令旗在，你且进去试试运气。”\n", me);
			message_vision("$N点了点头，随两个家人进了大门。\n", me);
			if (!(dest = find_object(__DIR__"hall")))
				dest = load_object(__DIR__"hall");
			if (!present("shi lingwei", dest))
				shi->move(dest);
			else
				destruct(shi);
			if (!present("ding jian", dest))
				ding->move(dest);
			else
				destruct(ding);
			me->set_temp("plummanor/flag", 3);
//			destruct(inv[i]);
			me->move(dest);
		}
	}
	return 1;
}
