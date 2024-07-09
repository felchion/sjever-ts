// beike.c 贝壳海螺类
// by Lklv for girls :)
// 2001.9.27

#include <ansi.h>
inherit ITEM;

void create()
{
        switch(random(10)) {
               case 0 : set_name(HIY"海螺"NOR, ({ "hai luo", "hailuo", "luo" }) ); break;
               case 1 : set_name(HIM"鸳鸯贝"NOR, ({ "yuanyang bei", "beike", "bei" }) ); break;
               case 2 : set_name(HIR"珍珠贝"NOR, ({ "zhenzhu bei", "beike", "bei" }) ); break;
               case 3 : set_name(HIW"扇贝"NOR, ({ "bei ke", "beike", "bei" }) ); break;
               case 4 : set_name(HIG"鹦鹉螺"NOR, ({ "yingwu luo", "hailuo", "luo" }) ); break;
               case 5 : set_name(HIW"荷花贝"NOR, ({ "hehua bei", "beike", "bei" }) ); break;
               case 6 : set_name(YEL"花瓶螺"NOR, ({ "huoping luo", "hailuo", "luo" }) ); break;
               case 7 : set_name(HIW"六角螺"NOR, ({ "liujiao luo", "hailuo", "luo" }) ); break;
               case 8 : set_name(HIC"夜光螺"NOR, ({ "yeguang luo", "hailuo", "luo" }) ); break;
               case 9 : set_name(HIR"彩"HIY"虹"HIB"贝"NOR, ({ "caihong bei", "beike", "bei" }) ); break;
        }
	set("long", "一个色彩鲜艳的"+name()+"。\n");
	set_weight(200+random(100));
        set("value", 250+random(500));
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "个");
        }
        setup();
}