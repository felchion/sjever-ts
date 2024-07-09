//flowray 2005.6.20
#include <ansi.h>

inherit ITEM; 

void create()
{
	set_name (HIC"寒铁锤"NOR, ({ "hchui"}));

	set("unit","个");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	
	//set("rest",3);
	set("desc","可以用于强化升级高级玩家武器。");
	set("flag","pm_"+query("id"));
	set("credit",1000);	
	setup();
}

int restrict() {
	return 1;
}


int do_read()
{
	object obj,me=this_player();

	obj = new("/d/zhiye/obj/tiechui");
	obj->set_level(100);
  if(!obj || !obj->set("gift_target",me->query("id")) || !obj->move(me))  
	return notify_fail("生成道具出错，请报告巫师!\n");

	destruct(this_object());
	return 1;
}

void init()
{
	do_read();  
}
