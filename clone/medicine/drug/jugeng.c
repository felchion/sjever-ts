// /clone/drug/hugu.c

#include <ansi.h>
inherit ITEM;

void create()
{
        set_name(HIW"菊梗"NOR, ({"ju geng" }));
        set_weight(200);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "棵");
                set("value", 1500);
                set("medicine", "drug");
		set("make/drug", 1);
        }
	setup();
}
