// lengyue_dao.c 冷月宝刀
#include <ansi.h>
#include <weapon.h>
inherit BLADE;

void create()
{
	set_name( HIB"冷月宝刀"NOR , ({ "lengyue dao","blade","dao"}));
	set_weight(20000);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "柄");
		set("long", HIB"刀光如水，在冷月下流转不定。刀柄上用金丝银丝镶着一钩眉毛弯月。\n"NOR);
		set("value", 1);
		set("unique", 1);
		set("rigidity", 4);
		set("sharpness", 4);
		set("weapon_prop/parry", 4);
		set("wield_maxneili", 1400);
		set("treasure",1);
		set("wield_str", 24);
		set("weapon_prop/ap", 80);
		set("material", "steel");
		set("wield_msg", HIB"$N缓缓抽刀出鞘，冷森森的一道青光激射而出，眨眼之间，手中已多了一柄寒气逼人的长刀。\n"NOR);
		set("unwield_msg",HIB"$N将冷月宝刀小心的收起，耀人的寒光没入刀鞘。\n"NOR);
	}
	init_blade(150);
	setup();
}
