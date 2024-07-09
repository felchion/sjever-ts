// paper.c 白纸
// By River@SJ 

inherit ITEM;
#include <ansi.h> 

void create()
{
	set_name(HIW"白纸"NOR, ({ "bai zhi", "bai", "zhi" }));
	set_weight(5);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "张");
		set("long", "一张干干净净的白纸，看来可以用它来折叠些什么。\n");
		set("value", 100);
		set("material", "paper");
	}
	setup();
}
