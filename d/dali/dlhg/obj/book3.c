//泥潭中国
//http://mud.lovefinal.com
//Final(章鹏)
// book1.c 天龙八部3
#include <ansi.h>
inherit ITEM;

void create()
{
	set_name(HIW"夜叉篇"NOR, ({"yecha pian", "shu","book"}));
	set_weight(80);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "这是天龙八部的之三--夜叉篇。\n");
		set("unit", "本");
	     }
}
