// Write by Yanyang@SJ 2000.9.23
// jinbei.c  小金杯

inherit ITEM;
inherit F_LIQUID;
#include <ansi.h>

void create()
{
        set_name(HIY"小金杯"NOR, ({"xiao jinbei", "jinbei", "bei"}));
        set_weight(700);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", "一只用金子打造的酒杯，金光闪闪，贵气逼人。\n");
                set("unit", "只");
                set("value", 3000);
                set("max_liquid", 1);
        }

        set("liquid", ([
                "type": "alcohol",
                "name": "女儿红",
                "remaining": 1,
                "drunk_apply": 50,
        ]));
}
