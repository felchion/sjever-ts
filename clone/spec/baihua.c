//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (HIM"百花丸"NOR, ({"baihua wan","wan"}));
	set("long",HIW"采集百花之蕊精心练制的疗伤秘药。\n"NOR);

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/baihua");
	set("desc","可以治疗各种顽固的内伤。");
	set("credit",3);	
	setup();
}

void init()
{
	add_action("do_fu","fu");
  
}

int do_fu(string arg)
{
	object me=this_player();
      	
      		
      	if (arg!="wan")
      		return notify_fail("你要服用什么?\n"); 
	
	if (me->is_busy()) {return notify_fail("你正忙着呢。\n");}
			
	if((!mapp(me->query_conditions_by_type("hurt"))) && (!mapp(me->query_conditions_by_type("wound"))))
		{return notify_fail("你好象没有受到什么内伤吧。\n");}
	
	me->clear_conditions_by_type("hurt");
	me->clear_conditions_by_type("wound");
		
	write(HIW"你服一下颗"HIM"百花丸"HIW"觉得一股香气沁入心脾。\n你的内伤痊愈了。\n"NOR);
	degree();
	return 1;
}	 