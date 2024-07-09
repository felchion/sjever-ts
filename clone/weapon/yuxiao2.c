// yuxiao.c 玉箫

#include <ansi.h>
#include <weapon.h>
inherit SWORD;

void create()
{
        set_name(HIG"玉箫"NOR, ({ "yu xiao", "xiao" }));
        set_weight(500);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "根");
                set("long", "这箫通身碧绿，竟是上好的翠玉，近吹口处有几点朱斑，殷红如血，更映得玉箫青翠欲滴。\n");
                set("value", 2500);
                set("no_sell", 1);
                set("material", "steel");
                set("wield_msg", "$N随意拿起一根$n握在手中。\n");
                set("unwield_msg", "$N手腕一转，将手中的$n收了起来。\n");
        }
        init_sword(35);
        setup();
}
