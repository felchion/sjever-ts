// changshan2.c
// Lklv 2001.9.25

#include <ansi.h>
#include <armor.h>
inherit CLOTH;

void create()
{
        set_name(HIY"黄色长衫"NOR, ({ "changshan", "cloth" }) );
        set_weight(400);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "件");
                set("material", "cloth");
                set("long", HIY"一件青年男子穿的黄色长衫。\n"NOR);
                set("value", 3500);
                set("armor_prop/armor", 2);
        }
        setup();
}