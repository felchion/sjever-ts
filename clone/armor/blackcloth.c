// blackcloth.c 黑衣

#include <armor.h>

inherit CLOTH;

void create()
{
        set_name("黑衣", ({ "hei yi", "black cloth", "cloth" }) );
        set_weight(400);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "件");
                set("material", "cloth");
                set("armor_prop/armor", 1);
        }
        setup();
}
