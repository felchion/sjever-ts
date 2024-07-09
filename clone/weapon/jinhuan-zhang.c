#include <ansi.h>
#include <weapon.h>

inherit STAFF;
inherit F_UNIQUE;
void create()
{
        set_name(HIY "九环锡杖" NOR, ({ "jiuhuan xizhang", "xizhang", "jiuhuan","staff" }));
        set_weight(32000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "根");
                set("long", "这是根佛家降魔护法至宝九环锡杖。但见：\n"HIY"
  铜镶铁造九连环，九节仙藤永驻颜。入手厌看青骨瘦，下山轻带白云还。
  摩呵五祖游天厥，罗卜寻娘破地关。不染红尘些子秽，喜伴神僧上玉山。\n"NOR);
                set("value", 10000);
                set("unique", 1);
                set("rigidity", 4);
                set("material", "gold");
                set("weapon_prop/parry", 3);
                set("treasure",1);
                set("wield_neili", 800);
                set("wield_maxneili", 1300);
                set("wield_str", 24);
                set("wield_msg", HIY"$N将九环锡杖往地上一柱，只见杖上九环相撞齐鸣，光芒四射！\n"NOR);
                set("unwield_msg", HIY"$N将手中的九环锡杖用白绸仔细包裹起来。\n"NOR);
        }
        init_staff(60);
        setup();
}
