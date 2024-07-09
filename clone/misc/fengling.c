// fengling.c 贝壳风铃
// By River@SJ 

inherit ITEM;
#include <ansi.h> 

void create()
{
	set_name(MAG"贝壳风铃"NOR, ({ "fengling", "ling" }));
	set_weight(200);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "串");
		set("long", "一串用各色海贝串成的风铃,五光十色,风一吹,发出悦耳的叮铛的乐音。\n");
		set("value", 1300);
		set("material", "shell");
	}
	setup();
}

void init()
{
	add_action("do_bo", "bo");
}

int do_bo(string arg)
{
	object me =this_player();
	string color;
	string *colors=({HIY,MAG,HIW,HIG,HIM,CYN,HIR,WHT});

        if (!id(arg))  return 0;

	if ( me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");
	
        color = colors[random(sizeof(colors))];

        message_vision(color+"\n$N用手指轻轻地抚弄着风铃,思绪也仿佛随着铃音飘向远方。\n"NOR, me); 
        me->add_busy(1);
	return 1;
}
