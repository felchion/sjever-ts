#include <armor.h>
#include <ansi.h>
inherit BOOTS;

void create()
{
        set_name(HIW"白色轻靴"NOR, ({ "shoes" }) );
        set_weight(2000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "双");
                set("material", "boots");
                 set("armor_prop/dodge", 1);
        }
        setup();
}