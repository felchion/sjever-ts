// shoe.c ?踏雪无痕 
#include <armor.h>
inherit BOOTS;
#include <ansi.h>

void create()
{
        set_name(GRN"踏雪无痕"NOR, ({ "shoes", "xie" }) );
        set_weight(1000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "双");
                set("long","这是一双绿色的布制快靴。\n");
                set("value", 0);
                set("material", "boots");
                set("armor_prop/dodge", 1);
              }
         setup();
}

