//泥潭中国
//http://mud.lovefinal.com
//Final(章鹏)
// mixian.c 过桥米线
#include <ansi.h>
inherit ITEM;
inherit F_FOOD;

void create()
{
	set_name(HIW"过桥米线"NOR, ({"guoqiao mixian","mixian"}));
	set_weight(80);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "一碗热乎乎的过桥米线。\n");
		set("unit", "碗");
		set("value", 500);
		set("food_remaining", 3);
		set("food_supply", 80);
	}
}
