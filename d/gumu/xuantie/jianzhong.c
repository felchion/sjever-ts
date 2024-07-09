// jianzhong.c
// By River 98.08.29
inherit ROOM;
#include <ansi.h>
void create()
{
	set("short", HIW"剑冢"NOR);
	set("long", @LONG
搬开冢上石块，露出并列着的三柄长剑。在第一、第二两把剑之间，另有
一块长条石片(shipian)。三柄剑和石片并列于一块大青石之上。
LONG
	);
	set("item_desc", ([
		"shipian" : "紫薇软剑，三十岁前所用，误伤义不祥，乃弃之深谷。\n",
	]));

	setup();
}

void init()
{
	add_action("do_ti", "ti");
	add_action("do_ti", "na");
	add_action("do_tui", "tui");
}

void reset()
{
	::reset();
	set("wmbj", 1);
	set("xtj", 1);
}

int do_ti(string arg)
{
	object me,sword,ob;
	me = this_player();
	if (!arg)
		return 0;
	if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢！\n");
	if (!present("fire", me))
		return notify_fail("你身上没有点火之物，看得清楚提什么吗？\n");
	if (arg == "gangjian") {
		if (!query("wmbj"))
			return notify_fail(HIR"你来晚了，无名宝剑已经给人取走了。\n"NOR);
		ob = unew(BINGQI_D("sword/wuming-jian"));
		if (!clonep(ob))
			return notify_fail(HIR"你来晚了，无名宝剑已经给人取走了。\n"NOR);
		tell_object(me,HIY"你提起右首第一柄剑，只见剑下的石上刻有两行小字：\n"+
			"「凌厉刚猛，无坚不摧，弱冠前以之与河朔群雄争锋。」\n"NOR);
		tell_room(environment(me), me->name() + "附下身躯，伸手提起右首第一柄剑。\n"NOR, ({ me }));
		delete("wmbj");
		ob->move(me);
		return 1;
	} else if (arg == "xuantiejian") {
		if (!me->query("ygpass")) {
			if (me->query_str() < 35 )
				return notify_fail("你用力想提起玄铁剑，但力不从心。\n");
		} else if (me->query_str() < 32)
			return notify_fail("你用力想提起玄铁剑，但力不从心。\n");
		if (me->query("max_neili") < 2000 ) {
			tell_object(me,HIY"你伸手去提第二柄剑，只提起数尺，呛啷一声，竟然脱手掉下，在石上一碰，\n"+
				"火花四溅，不禁吓了你一跳。\n"NOR);
			tell_room(environment(me), me->name() + "伸手去提第二柄剑，只提起数尺，呛啷一声，竟然脱手掉下。\n"NOR, ({ me }));
			return 1;
		}
		if (!query("xtj"))
			return notify_fail(HIR"你来晚了，玄铁剑已经给人取走了。\n"NOR);
		sword= unew(BINGQI_D("sword/xuantiejian"));
		if(!clonep(sword))
			return notify_fail(HIR"你来晚了，玄铁剑已经给人取走了。\n"NOR);
		if(me->query("neili") >= 300 )
			me->add("neili", -300);
		else
			me->set("neili", 0);
		tell_object(me,HIY"于是你再俯提起，这次有了防备，提起七八十斤的重物自是不当一回事。见那\n"+
				  "剑两边剑锋都是钝口，剑尖更圆圆的似是个半球。看剑下的石刻时，见两行小\n"+
				  "字道：「重剑无锋，大巧不工。四十岁前恃之横行天下。」\n"NOR);
		tell_room(environment(me), me->name() + "吸了口气，伸手提起第二柄剑。\n"NOR, ({ me }));
		delete("xtj");
		sword->move(me);
		return 1;
	} else if( arg == "mujian") {
		tell_object(me,HIY"你力运左臂去取第三柄剑，哪知拿在手中却轻飘飘的浑似无物，凝神一看，原\n"+
			"来是柄木剑，年深日久，剑身剑柄均已腐朽，但见剑下的石刻道：「四十岁后，\n"+
			"不滞于物，草木竹石均可为剑。自此精修，渐进於无剑胜有剑之境。」\n"NOR);
		tell_room(environment(me), me->name() + "力运左臂，伸手去提第三柄剑。\n"NOR, ({ me }));
		return 1;
	}
}

int do_tui(string arg)
{
	object me;
	me = this_player();
	if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢！\n");
	if ( !arg || arg !="shi" )
		return notify_fail("你要推什么？\n");
	message_vision(YEL"$N深深地吸了口气，缓缓推动青石。\n"NOR,me);
	write(HIG"\n你把青石推开！露出了剑冢的出口。\n"NOR);
	set("exits/out", __DIR__"pingtai");
	remove_call_out("close");
	call_out("close", 5, this_object());
	return 1;
}

void close(object room)
{
	message("vision",HIY"青石慢慢移回了原位，又挡住了出口。\n"NOR, room);
	room->delete("exits/out");
}
