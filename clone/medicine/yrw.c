// qiannianshouwu.c 千年何首乌

#include <ansi.h>
inherit ITEM;

void setup()
{}

void create()
{
        set_name(WHT"人参养容丸"NOR, ({"yangrong wan", "wan"}));
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "颗");
                set("long", "这是一颗"WHT"人参养容丸"NOR"据说吃了之后有美容养颜的神奇功效。\n");
                set("unique", 1);
                set("drug", 1);
                set("treasure", 1);
                set("value", 10000);
        }
        setup();
}

void init()
{
        add_action("do_eat", "eat");
}
 
void effect(object me) {
	if (!me || !objectp(me)) return;
	tell_object(me,HIY"你运功完毕，发现自己好象漂亮了一点！\n"NOR);
	destruct(this_object());
	return;
}

int do_eat(string arg)
{
	object me = this_player();
	mapping fam = me->query("family");
	
	if (arg!="wan" && arg!="yangrong wan")
		return notify_fail("你要吃什么？\n");

	if (!fam || fam["family_name"] != "古墓派")
		return notify_fail("人参养容丸与你所习内功相悖，还是不要吃了吧。\n");
	
	if(me->query("per")>24) 
		return notify_fail("你天生丽质何须用药？\n");
		
	if(me->query("age")<16)
    return notify_fail("你年龄太小，无法承受人参养容丸的药性。\n");
   
  if(me->query("max_neili")<800)
    return notify_fail("你内力修为太低，无法承受人参养容丸的药性。\n");
  
  me->set("per",25+random(3));
  me->add("max_neili",-(me->query("per")-23)*200);
  call_out("effect", 5, me);
  tell_object(me,"你吃下一颗"WHT"人参养容丸"NOR"，运起内功缓缓驱动药性转遍全身。\n");
  me->start_busy(5);  
	return 1;
}

