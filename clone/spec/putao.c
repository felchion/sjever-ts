//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (HIG"神奇葡萄"NOR, ({ "putao"}));
	set("long","这是一颗"HIG"神奇葡萄"NOR"，吃(eat)下它会发生奇怪的事情。\n");

	set("unit","串");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/putao");
  set("rest",20);
	set("desc","增加潜能一千点。");
	set("credit",3);
	setup();
}

void init()
{
	add_action("do_eat","eat");
  
}

int do_eat(string arg)
{
	object me=this_player();
	
	    
	if (arg!="putao")
      		return notify_fail("你要吃什么?\n");
      	
      	if (!restrict()) {return notify_fail("本周你已经不能再吃"+this_object()->query("name")+"了。\n");}   

	me->add("potential",1000);
	write(HIY"你吃下一颗"HIG"神奇葡萄"HIY"发现自己的潜能大增!\n"NOR);
	degree();
	return 1;
}	 