// tiewhip 铁鞭

#include <weapon.h>
inherit WHIP;

void create()
{
        set_name("铁鞭", ({ "tie bian", "tiebian","bian","whip" }));
        set_weight(15000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "柄");
                set("long", "这是一柄铁制的长鞭，异常沉重。\n");
                set("value", 500);
                set("material", "steel");
                set("wield_msg", "$N「唰」的一声抖出一柄$n握在手中。\n");
                set("unwield_msg", "$N将手中的$n卷回腰间。\n");
        }
        init_whip(30);
        setup();
}

