// item: /d/xingxiu/npc/obj/fire.c
// Jay 3/17/96
#include <ansi.h>
inherit ITEM;
void create()
{
        set_name(HIR"火折"NOR, ({"fire", "huozhe"}));
        set_weight(80);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", "这是一支用于引火的火折，到了黑暗的地方就用得着了。\n");
                set("unit", "支");
                set("value", 500);
                set("no_sell", 1);
        }
        setup();
}
