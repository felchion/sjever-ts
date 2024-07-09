// bamboo_stick.c 玉竹棒
#include <ansi.h>
#include <weapon.h>
inherit STICK;
inherit F_UNIQUE;

void create()
{
        set_name(GRN"玉竹棒"NOR, ({"yuzhu bang", "bang", "stick"}));
        set_weight(5000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "根");
                set("long", "一根泛着淡淡青光的竹杖。\n");
                set("value", 1000);
                set("unique", 1);
                 set("rigidity", 9);
                set("material", "bamboo");
                set("weapon_prop/parry", 10);                
                set("wield_neili", 100);
                set("wield_maxneili", 500);
                set("wield_str", 18);                
                set("wield_msg", "$N抽出一根青光闪闪的玉竹杖抓在手中。\n");
                set("unwield_msg", "$N把手中的玉竹杖插回腰中。\n");
                set("treasure",1);
        }
     		init_stick(160);
        setup();
}       
