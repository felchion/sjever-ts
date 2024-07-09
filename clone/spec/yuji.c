//flowray 2005.6.16
#include <ansi.h>


inherit SPEC;

void create()
{
	set_name (HIW"玉肌丸"NOR, ({ "yuji wan","yuji","wan"}));
	set("long","这是一颗"HIW"玉肌丸"NOR"，你可以服(fu)下它提高精力和内力修为。\n");

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/yuji");
	set("rest",3);
	set("desc","提高一百点内力、一百点精力的上限。");
	set("credit",10);
	setup();
}

void init()
{
	add_action("do_eat","eat");
	add_action("do_eat","fu");    
}

int do_eat(string arg)
{
	object me=this_player();
	
	if (!arg)
		return notify_fail("你要服用什么?\n");
	    
	if (arg!="yuji" && arg!="wan")
		return notify_fail("你要服用什么?\n");
      	
	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}
    
	me->add("max_neili", 100);
	me->add("max_jingli", 100);
	me->add("eff_jingli", 100);
	write(HIY"你服下一颗"HIW"玉肌丸"HIY"顿觉精力内力大增!\n"NOR);
	degree(); 
	return 1;
}	 