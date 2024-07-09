// bishou.c 匕首

#include <weapon.h>
inherit DAGGER;

void create()
{

        set_name("匕首", ({ "bishou"}));
        set_weight(2300);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "柄");
                set("long", "这是一柄匕首。\n");
                set("value", 3000);
                set("material", "steel");
        }
        init_dagger(30);
        setup();
}
