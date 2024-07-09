// fengche.c 纸风车
// By River@SJ 

inherit ITEM;
#include <ansi.h> 

void create()
{
	set_name(HIC"纸风车"NOR, ({ "zhi fengche", "zhi", "fengche" }));
	set_weight(200);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "架");
		set("long", "一架用彩纸折叠的风车,十分精巧。\n");
		set("value", 1000);
		set("material", "paper");
	}
	setup();
}

void init()
{
	add_action("do_blow", "blow");
}

int do_blow(string arg)
{
	object me =this_player();
	string color;
	string *colors=({HIY,MAG,HIW,HIG,HIM,CYN,HIR,WHT});

        if (!id(arg))  return 0;

	if ( me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");

        color = colors[random(sizeof(colors))];

	switch (random(2)){
		case 0:	message_vision(color+"\n$N轻轻地吹了下风车，风车轻轻转动起来,在阳光下折射着彩色的光。\n"NOR, me); break;
		case 1: message_vision(color+"\n$N鼓起腮帮轻轻地吹着风车,风车沙沙地转动起来.....\n"NOR,me); break;
	}
        me->add_busy(1);
	return 1;
}
