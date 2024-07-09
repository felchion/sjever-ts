// ningxue.c 凝血神爪谱
// By River@SJ 2003.1.11

inherit ITEM;
#include <ansi.h>

void create()
{
	set_name(HIR"凝血神爪谱"NOR, ({ "ningxue shenzhuapu", "pu", "book"}));
	set_weight(500);
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("unit", "本");
		set("long", "这就是天地会总舵主陈近南的绝技凝血神爪的图谱。\n");
		set("value", 5000);
		set("no_give", 1);
		set("treasure",1);
		set("material", "paper");
		set("no_get", "这样东西不能离开那儿。\n");
	}
	setup();
}

void init()
{
	if (environment() == this_player())
		add_action("do_du", "read");
}

int do_du(string arg)
{
	object me = this_player();

	if (!id(arg)) 
		return notify_fail("你要读什么？\n");

	if ( me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");

	if ( !me->query("quest/ningxue/pass") )
		return notify_fail("你是怎么得到「凝血神爪谱」的啊？\n");

	if ( query("owner") != me )
		return notify_fail("你是怎么得到「凝血神爪谱」的啊？\n");

	if ( me->query("neili") < 1500 )
		return notify_fail("你的真气不够。\n");

	if (environment(me)->query("pigging"))
		return notify_fail("你还是专心拱猪吧！\n");

	if (environment(me)->query("pending"))
		return notify_fail("此地不宜研习「凝血神爪谱」！\n");

	if (environment(me)->query("sleep_room"))
		return notify_fail("卧室不宜修练，会影响别人休息。\n");
 
	if (environment(me)->query("no_fight"))
		return notify_fail("此地不宜研习「凝血神爪谱」！\n");

	if ( me->query("jing") < 40 )
		return notify_fail("你的精太少了，不够研读「葵花宝典」。\n");

	if ( me->query_skill("ningxue-shenzhua", 1) > 150 )
		return notify_fail("你研读了一会儿，但是发现上面所说的对你而言都太浅了，没有学到任何东西。\n");

	tell_object(me, "你仔细研读着「凝血神爪谱」的精妙之处。\n");
	me->receive_damage("jing", 30 );
	me->add("neili", - 50);
	me->improve_skill("ningxue-shenzhua", me->query_int());
	if (!random(8))
		message("vision", me->name() + "仔细研读着「凝血神爪谱」的精妙之处。\n", environment(me), me); 
	return 1;
}
