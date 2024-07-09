#include <ansi.h>

inherit ITEM; 

void create()
{
	set_name (CYN"洛迦之心"NOR, ({ "lheart","heart"}));
	set("long","这是一颗"HIC"洛迦之心"NOR"，使用("HIY"use"NOR")它能让你专致于武学。\n");

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);

	set("desc","两个小时之内经验加倍。");
	set("credit",10);	
	setup();
}

int do_use(string arg);

void init()
{
	do_use("");  
}

int restrict() {
  object me=this_player();
  
  if(me->query_condition("smy_job") && !me->query_condition("qt_exp"))
  return notify_fail("你现在无法使用"+this_object()->query("name")+"。\n");
  
  return 1;
}

int do_use(string arg)
{
	object me=this_player();

	if (!restrict()) {return notify_fail("你现在不能使用"+this_object()->query("name")+"了。\n");}

	me->add_condition("qt_exp", 360);

	write(HIY"你使用洛加之心，只觉心念懵懂痴愚，一时间专致武学不可自拔。\n"NOR);

  log_db(me,0,0,0,"洛迦之心");
  
  destruct(this_object());  

	return 1;
}

