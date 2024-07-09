// ITEM: /d/huashan/npc/obj/bishuijian.c
// Date: Look 99/03/25

#include <weapon.h>
#include <ansi.h>
inherit SWORD;

void create()
{
	set_name( HIG "碧水剑" NOR, ({ "bishui jian", "jian", "sword" }));
	set_weight(12000);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "柄");
		set("long", GRN "这是柄又薄又轻的软剑，剑身上隐隐的透出绿光，当真锋利无比。\n" NOR);
		set("value", 1500);
		set("rigidity", 3);
		set("sharpness", 3);
		set("weapon_prop/parry", 2);
		set("material", "steel");
		set("unique", 1);
		set("treasure",1);
		set("wield_neili", 500);
		set("wield_maxneili", 900);
		set("wield_str", 22);
		set("weapon_prop/ap", 40);
		set("wield_msg", GRN "只听「嗡」地一声，绿光顿现，$N在腰间解下$n"+ GRN "，「刷」的一声抖得笔直。\n" NOR);
		set("unwield_msg", GRN "只见绿光一闪，你的眼前一花，$N将手中的$n"+ GRN "缠在腰间。\n" NOR);
	}
	init_sword(100);
	setup();
}
mixed hit_ob(object me, object victim, int damage_bonus)
{
	int n;

	if (me->query_skill_mapped("sword") != "huashan-jianfa" ||
	me->query_skill("huashan-jianfa", 1) < 100)
	return damage_bonus / 2;

	switch (random(6))
	{
		case 0:
		if (! victim->is_busy())
		victim->start_busy(me->query_skill("sword") / 300 + 1);
		return HIG "碧水剑泛出幽幽绿光，如丝丝细带缠向$n。\n"NOR;



		case 1:
		n = me->query_skill("sword");
		victim->receive_damage("qi", n , me);
		victim->receive_wound("qi", n/3 , me);
		victim->receive_damage("neili", n , me);

		return     HIG "碧水剑激射出一道青芒，闪电般攻向$n空门处的大穴。\n" NOR;


	}
	return damage_bonus;
}
