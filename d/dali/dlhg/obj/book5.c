//泥潭中国
//http://mud.lovefinal.com
//Final(章鹏)
// book1.c 天龙八部5
#include <ansi.h>
inherit ITEM;

void create()
{
	set_name(HIW"阿修罗篇"NOR, ({"axiuluo pian","shu","book"}));
	set_weight(80);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "这是天龙八部的之五--阿修罗篇。\n");
		set("unit", "本");
	     }
}
