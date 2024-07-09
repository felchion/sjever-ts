// yujiwan.c 玉肌丸
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
	set_name(HIW"玉肌丸"NOR, ({"yuji wan", "yuji", "wan"}));
	set_weight(300);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "颗");
		set("no_give", 1);
		set("long", "这是一颗晶莹剔亮的玉肌丸，看来可以服用(eat)。\n");
		set("value", 10000);
		set("drug", 1);
		set("no_sell", 1);
		set("no_get",1);
		set("no_drop",1);
		set("medicine", "drug");
		set("make/drug", 1);
	}
	setup();
}
 
int do_eat(string arg)
{
	object me = this_player();

	if (!id(arg)) return notify_fail("你要吃什么？\n");

	if( query("owner") != me->query("id"))
		return notify_fail("你是怎么得到这颗玉肌丸的？\n");

	message_vision(HIY"$N服下一颗"+query("name")+HIY+"，顿时觉得精力和内力大增，浑身精气十足。\n"NOR,me);
	me->add("max_neili", 100);
	me->add("max_jingli", 100);
	me->setup();
	destruct(this_object());
	return 1;
}
