// baihehua.c
// creat by Lklv@SJ for 各位美女 @@

#include <ansi.h>;
#include <armor.h>;
inherit HEAD;

void create()
{
	set_name(HIW"百合花"NOR, ({"baihe hua", "hua", "flower" }));
	set_weight(10);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "朵");
                set("long", "这是一朵美丽的百合花，香气清新。\n");
                set("value", 110);
                set("material", "plant");
                set("armor_prop/armor", 1);
                set("armor_prop/personality", 1);
                set("wear_msg", "$N轻轻地把一朵$n戴在头上。\n");
                set("unequip_msg", "$N轻轻地把$n从头上摘了下来。\n");
        }
        setup();
}
