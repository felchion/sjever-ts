#include <armor.h>
#include <ansi.h>
inherit NECK;
inherit F_UNIQUE;

void create()
{
        set_name(HIY"雄黃珠子"NOR, ({ "xionghuang zhuzi", "zhuzi","hate snake's zhu"}));
        set_weight(5000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", "这是一串微微泛着淡黄色的珠子，据说戴在身上，百毒不侵。\n");
                set("material", "silk");
                set("unit", "只");
                set("unique", 1);
                set("value", 1000000);
                set("wear_msg", HIY "$N从身上摸出一串淡黄色的珠子戴起来。\n" NOR);
                set("remove_msg", HIY "$N除下雄黄珠子，小心翼翼的放在贴身处。\n" NOR);
        set("armor_prop/neck", 50);
        }
        setup();
}