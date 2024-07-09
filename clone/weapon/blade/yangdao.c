// yang.c 鸯刀
// zly 99.6.22

#include <weapon.h>
#include <ansi.h>

inherit BLADE;
inherit F_UNIQUE;

void create()
{
	set_name(HIC"鸯刀"NOR, ({"yang dao","yang","dao", "blade"}));
	set_weight(10000);
	if( clonep() )
	set_default_object(__FILE__);
	else{
		set("treasure", 1);
		set("unit", "把");
		set("long", HIC"这是鸳鸯刀中的鸯刀，但见精光耀眼，刀刃上刻著「无敌」二字。\n" NOR);
		set("value",4000);
		set("material","steel");
		set("weapon_prop/parry",2);
		set("unique", 1);
		set("rigidity", 3);
		set("sharpness", 3);
		set("wield_neili", 500);
		set("wield_maxneili", 900);
		set("weapon_prop/ap", 40);
		set("wield_str", 22);
		set("wield_msg", HIC"$N抽出一把青光闪耀，寒气逼人的长刀握在手中。心中豪气顿生，口中不觉轻啸一声。\n" NOR);
		set("unwield_msg", HIC"$N轻轻将手中的鸯刀插入腰间的刀鞘。\n" NOR);
	}
	init_blade(80);
	setup();
}
