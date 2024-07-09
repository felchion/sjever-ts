// chuanliu-dao.c 飞雁穿柳刀

#include <ansi.h>
#include <weapon.h>
inherit BLADE;

void create()
{
	set_name( HIC"飞雁穿柳刀"NOR , ({ "chuanliu dao", "dao", "blade" }));
	set_weight(12500);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "把");
		set("long", HIC"这是一把薄薄的软刀，刀身轻柔有韧，散发出阵阵绚丽的蓝光。\n"NOR);
		set("value", 10000);
		set("unique", 1);
		set("rigidity", 3);
		set("weapon_prop/parry", 5);
		set("wield_neili", 100);
		set("wield_maxneili", 900);
		set("wield_str", 20);
		set("treasure",1);
		set("material", "steel");
		set("weapon_prop/ap", 80);
		set("wield_msg", HIC"$N手腕一翻一抖，瞬间手中已多了一把软刀。\n"NOR);
		set("unwield_msg",HIC" $N随手一抖，飞雁穿柳刀顿时失去了踪影。\n"NOR);
	}
	init_blade(140);
	setup();
}


