// ruanxue2.c
// build by Lklv 2001.9.25

#include <ansi.h>
#include <armor.h>
inherit BOOTS;

void create()
{
        set_name(WHT"软底快靴"NOR, ({ "ruan xue", "xue", "xuezi", "shoes" }) );
        set_weight(100);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "双");
		set("value", 1550);
                set("material", "boots");
                set("armor_prop/dodge", 6);
                set("long", WHT"一双缎子面的软底快靴，穿在脚上特别轻快。\n"NOR);
        }
        setup();
}
