// yaoqin.c 瑶琴

#include <weapon.h>
#include <ansi.h>

inherit SWORD;

void create()
{
        set_name(YEL"瑶琴"NOR,({"yao qin","qin"}));
        set_weight(1000);
        if(clonep())
                set_default_object(__FILE__);
        else{
                set("unit","张");
                set("value",1000);
                set("material","steel");
                set("unique", 1);
                set("rigidity", 5);
                set("treasure",1);
                set("long","这张瑶琴颜色暗旧，当是数百年甚至是千年以上的古物。\n");
                set("wield_msg",YEL"$N伸手一拂将瑶琴捧在手中,在琴弦上拨了几下。\n"NOR);
                set("unwield_msg",YEL"$N手腕一转，琴音停了下来。\n"NOR);
        }
        init_sword(42);
        setup();
}
