// Write By Yanyang@SJ 2001.9.27
// F_shuimitao.c

inherit ITEM;
inherit F_FOOD;
#include <ansi.h>
 
void create()
{
        set_name(HIM"水蜜桃"NOR, ({"shuimi tao", "shuimitao", "tao"}));
        set_weight(40);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", "一颗香甜的水蜜桃。\n");
                set("unit", "颗");
                set("value", 40);
                set("food_remaining", 2);
                set("food_supply", 25);
          }
        setup();
}
