// /clone/drug/hua.c

#include <ansi.h>
inherit ITEM;

void create()
{
        set_name(HIW"金银花"NOR, ({"jinyin hua"}));
        set_weight(200);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "朵");
                set("value", 3500);
                set("medicine", "drug");
		set("make/drug", 1);
        }
	setup();
}
