//泥潭中国
//http://mud.lovefinal.com
//Final(章鹏)
// book1.c 天龙八部7
#include <ansi.h>
inherit ITEM;

void create()
{
	set_name(HIW"紧那罗篇"NOR, ({"jinnaluo pian", "shu","book"}));
	set_weight(80);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "这是天龙八部的之七--紧那罗篇。\n");
		set("unit", "本");
	     }
}
