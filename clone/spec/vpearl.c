//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (RED"幻视之珠"NOR, ({ "vpearl","zhu"}));
	set("long","这是一颗"RED"幻视之珠"NOR"，使用("HIY"use"NOR")它能拥有一双洞察之眼。\n");

	set("unit","本");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/vpearl");
	set("desc","十二个小时之可以看透敌人的技能属性。");
	set("credit",2);	
	setup();
}

int do_use(string arg);

void init()
{
	do_use("");
  
}

int do_use(string arg)
{
	object me=this_player();

	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}

	me->add_condition("true_view", 360*6);

	write(HIM"你将幻视之珠用力捏碎，珠子化为一团迷雾钻你了你的两眼。\n"NOR);

	degree();
	return 1;
}