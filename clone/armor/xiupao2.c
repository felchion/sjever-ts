// xiupao2.c
// Lklv 2001.9.25

#include <ansi.h>
#include <armor.h>
inherit CLOTH;

void create()
{
        set_name(HIR"大红绣袍"NOR, ({ "hongxiu pao", "xiupao", "pao" }) );
        set_weight(4000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "件");
                set("material", "cloth");
                set("long", HIR"一件喜气洋洋的大红色缎子长袍。\n"NOR);
                set("value", 12500);
                set("armor_prop/armor", 2);
        }
        setup();
}