#include <weapon.h>
#include <ansi.h>

inherit AXE;
void create()
{
        set_name(HIY"金龙夺"NOR,({ "jinlong duo", "duo","axe" }) );
        set_weight(20000);
        if( clonep() )
                set_default_object(__FILE__);
        else {

                set("unit", "把");
                set("value", 10000000);
                set("unique", 1);
                set("rigidity", 1);
                set("sharpness", 5);    
                set("material", "steal");
		set("treasure",1);
                 set("wield_maxneili", 1500);
                set("wield_str", 30);
		set("long", YEL"这是一把金龙形的上古神夺，挥舞起来，只见金光万道，溢满四周，又如真龙翻腾，
眩人神目，根本连使夺之人的样子也看不真切。"NOR);
                set("wield_msg",YEL"$N双眸精光陡射，右掌一探，一柄眩人心神，通体散发着波波金色光辉的金龙夺，已经握在手中。\n"NOR);             
                set("unwield_msg", YEL "$N手中金光一闪，金龙夺消失在$N的怀中。\n"NOR);               
        }
        init_axe(130);
        setup();
}

