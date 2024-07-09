// ziweijian 紫薇软剑

#include <weapon.h>
#include <ansi.h>

inherit SWORD;
//inherit F_UNIQUE;

void create()
{
	set_name(HIY"紫薇软剑"NOR,({"ziwei jian","ziwei","jian","sword"}));
	set_weight(15000);
	if( clonep() )
		 set_default_object(__FILE__);
	else {
		  set("unit", "柄");
                set("value", 1);
                //set("unique", 1);                
                set("rigidity", 10);
                set("sharpness", 9);
                set("material", "steel");
                set("weapon_prop/parry", 8);
                //set("treasure",1);
                set("wield_neili", 1000);
                set("wield_maxneili", 1500);
                set("wield_str", 28);
                set("wield_msg", HIY"$N唰的一声从腰间抽出紫薇软剑，宝剑游走不定，寒气逼人。\n"NOR);
                set("long", "这是一柄六尺长的软剑，剑柄上刻着‘紫薇’两个金字。\n当年被独孤大侠弃于山谷，不知为何却出现在这里。\n");
                set("unwield_msg", HIY "$N将紫薇剑轻轻一抖，随意的束于腰间。\n" NOR);
	}
	init_sword(160);
	setup();
}