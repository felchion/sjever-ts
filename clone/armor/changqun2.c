// OBJ : /d/xiangyang/npc/obj/changqun2.c 绣花长裙
// By Lklv 2001.9.26

#include <ansi.h>
#include <armor.h>
inherit CLOTH;

void create()
{
        set_name(HIR"红色长裙"NOR, ({"chang qun", "changqun", "qunzi"}) );
        set_weight(400);
        if(clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "件");
                set("long", HIR"一件大红色的绣花长裙。\n"NOR);
                set("material", "cloth");
                set("value", 4500);
                set("armor_prop/armor", 2);
        }
        setup();
}
