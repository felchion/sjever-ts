// zhaohun.c 招魂幡?

#include <weapon.h>
#include <ansi.h>
inherit WHIP;

void create()
{
        set_name(BLU"招魂幡"NOR, ({ "zhaohun fan", "fan","whip" }));
        set_weight(5000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "杆");
                set("long", "这是一杆插在灵座上的招魂幡。\n");
                set("value", 3000);
                set("rigidity", 5);
                set("material", "steel");
                set("wield_msg", BLU"$N抽出一柄招魂幡握在手中，模样诡奇无比，令人气为之夺。\n"NOR);
                set("unwield_msg", "$N将手中的$n别回腰后。\n");
        }
        init_whip(38);
        setup();
}

