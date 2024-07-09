//flowray 2005.6.20
#include <ansi.h>

inherit ITEM; 

void create()
{
	set_name (HIR"工匠锦囊"NOR, ({ "jinnang"}));

	set("unit","个");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	
	set("rest",3);
	set("desc","倚天屠龙技残篇一卷。");
	set("flag","pm_"+query("id"));
	set("credit",200);	
	setup();
}

void init()
{
	add_action("do_read","kai");  
}

int restrict() {
	 return 1;
}

int do_read(string arg)
{
	object obj,me=this_player();

	if (!arg)
	return notify_fail("你要开什么?\n");

	if (arg!="jinnang" && arg!="nang")
	return notify_fail("你要开什么?\n");

	obj = new("/d/zhiye/obj/book.c");
	obj->set_level(1+random(2));
  if(obj && obj->move(me))  
	write(HIW"你打开锦囊，发现里面有一卷"+obj->name()+HIW"。\n"NOR);
	else 
	return notify_fail("生成道具出错，请报告巫师!\n");

	destruct(this_object());
	return 1;
}