//泥潭中国
//http://mud.lovefinal.com
//Final(章鹏)
// book1.c 天龙八部1
#include <ansi.h>
inherit ITEM;

void create()
{
	set_name(HIW"天神篇"NOR, ({"tianshen pian","shu","book"}));
	set_weight(80);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "这是天龙八部的之一--天神篇。\n");
		set("unit", "本");
	     }
}
