// changshan5.c
// Lklv 2001.9.25

#include <ansi.h>
#include <armor.h>
inherit CLOTH;

void create()
{
        set_name(HIM"紫色长衫"NOR, ({ "changshan", "shan", "cloth" }) );
        set_weight(400);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "件");
                set("material", "cloth");
                set("long", HIM"一件男子穿的紫色长衫。\n"NOR);
                set("value", 3500);
                set("armor_prop/armor", 2);
        }
        setup();
}