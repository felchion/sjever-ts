// guzheng.c 古筝

#include <weapon.h>
#include <ansi.h>

inherit SWORD;

void create()
{
	set_name(HIG"古筝"NOR, ({ "gu zheng", "zheng", "qin" }));
	set_weight(5000);
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("unit", "张");
		set("long", HIG"这是一张做工十分考究的古筝，古香古色，音质极好。\n"NOR);
		set("value", 5000);
		set("no_sell", 1);
		set("material", "wood");
	}
	init_sword(30);
	setup();
}

void init()
{
	if (environment() == this_player())
		add_action("do_play", "play");
}

int do_play(string arg)
{
	object me = this_player();
	object env;

	if (!arg || !id(arg))
		return notify_fail("你想做什么？\n");

	if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");

	env = environment(me);
	if (env->query("sleep_room"))
		return notify_fail("卧室不能修练，会影响别人休息。\n");

	if (env->query("pigging"))
		return notify_fail("你还是专心拱猪吧！\n");

	if (env->query("pending"))
		return notify_fail("此地不宜修练！\n");

	if (env->query("no_fight"))
		return notify_fail("这里不准战斗，也不准修练。\n");

	if (me->query("qi") < 20 || me->query("jing") < 30)
		return notify_fail("你太累了，还是先休息会把。\n");
	if (random(10) > 7) {
		message_vision("$N摆出一张古筝，铮铮琮琮弹了几声只听得曲调杂乱无章，宛如鬼哭狼号。\n", me);
		me->receive_damage("jing", random(10));
		me->receive_damage("qi", random(10));
		return 1;
	}
	me->receive_damage("jing", 20);
	me->receive_damage("qi", 5);
	me->add_busy(!random(3));
	if (me->query_skill("art",1) < 120)
		me->improve_skill("art", me->query_int()/4 + me->query_skill("art", 1)/10);
	write("你摆出一张古筝，调了调弦，弹了起来，你只觉得连日来烦燥的心情好了许多。\n");
	tell_room(env, "只见" + me->name() + "雅兴大发，竟弹起曲子来，你只觉得时而金戈铁马、大漠平沙……\n"
		"时而小桥流水、几户人家……万般气象，你心情顿时宁静了许多。\n", me
	);
	return 1;
}
