// jiuhuawan.c 嵩阳丹
#include <ansi.h>

inherit ITEM;

void setup()
{}

void init()
{
	add_action("do_eat", "eat");
}

void create()
{
	set_name("嵩阳丹", ({"songyang dan", "dan"}));
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "颗");
		set("long", "这是一颗并不起眼的小圆球。\n");
		set("value", 100);
		set("no_drop", 1);
	}
	setup();
}

int do_eat(string arg)
{
	object me = this_player();

	if (!id(arg))
		return notify_fail("你要吃什么？\n");

	if ((int)this_player()->query("eff_qi") == (int)this_player()->query("max_qi"))
		return notify_fail("你现在不需要用药物。\n");
        
	if (this_player()->is_busy() || this_player()->is_fighting())
		return notify_fail("你正忙着呢。\n");

	if (me->query_condition("medicine"))
		return notify_fail("你刚服过良药，多吃无益。\n");

	if (me->query_skill_mapped("force") != "hanbing-zhenqi") {
		me->add("max_neili", -10);
		message_vision(HIR "$N吃下一颗嵩阳丹，只觉得丹田有如刀割，原来所练内功不符，反而大损真元！\n" NOR, me);
		me->unconcious();
		destruct(this_object());
		return 1;
	}
	else {
		me->receive_curing("qi", me->query_skill("hanbing-zhenqi", 1));
		me->add_condition("medicine", 5);
		message_vision("$N吃下一颗嵩阳丹，伤势大为好转。\n", this_player());
	}
	destruct(this_object());
	return 1;
}
