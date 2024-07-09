//iron-falun 铁法轮
// copy to hammer.c
//modified by campsun 2003/09/20
//轮子，最终威力最大的基本武器。
#include <ansi.h>
#include <weapon.h>
inherit F_HAMMER;
inherit F_EQUIP;
inherit COMBINED_ITEM;

void create()
{
        set_name( BLU"铁法轮"NOR,({ "iron falun" ,"lun" ,"falun","tie falun","tiefalun" }));
        set_weight(25000);
        if (clonep())
                set_default_object(__FILE__);
        else {
		set("unit", "只");
                set("long","这是一只铁制法轮，约有尺半大小，上面铸有密宗伏魔真言，内藏九个小球，是僧人们作法事用的一种法器。\n");
                set("value", 100);
                set("rigidity", 1);
                set("weapon_prop/dodge", -6);
                set("weapon_prop/parry", 2);    
                set("material", "iron");
                set("base_unit", "只");
                set("base_weight", 18000);
                set("wield_msg", "$N从怀中掏出$n掂了掂分量，握在手中，当啷啷响声良久。\n");
                set("unwield_msg","$N将手中的$n揣回怀中。\n");
        }    
        set_amount(5);           
        init_hammer(40);
        setup();
}


