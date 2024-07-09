#include <weapon.h>
#include <ansi.h>

inherit SWORD;

void create()
{
	set_name(HIG "碧玉剑" NOR, ({ "biyu jian", "sword", "jian" }) );
	set_weight(10000);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "把");
		set("value", 1000);
		set("treasure",1);
		set("rigidity", 4);
		set("sharpness", 6);
		set("unique", 1);
		set("weapon_prop/parry", 2);
		set("wield_neili", 500);
		set("wield_maxneili", 900);
		set("wield_str", 23);
		set("weapon_prop/ap", 60);
		set("material", "jade");
		set("long", "这是把前古神兵，剑身上隐隐的透出碧玉之光，当真锋利无比。\n");
		set("wield_msg", HIC "只听见「嗡」地一声，碧光闪现，$N将腰间解下的$n" HIC "抖得笔直。\n" NOR);
		set("unwield_msg", HIC "$n" HIC "幻作一道碧玉之光，「唰」地飞入剑鞘。\n" NOR);
	}
	init_sword(80);
	setup();
}
mixed hit_ob(object me, object victim, int damage_bonus)
{
	int n;

	if (me->query_skill_mapped("sword") != "murong-jianfa" ||
	me->query_skill("murong-jianfa", 1) < 100)
	return damage_bonus / 2;

	switch (random(8))
	{
		case 0:
		if (! victim->is_busy())
		victim->add_condition("no_exert", 2 + random(3));
		return HIG "碧玉剑发出一道阴寒剑气,$n只觉得剑气穿入身体,顿时身体内空荡荡的,非常难受。\n"NOR;



		case 1:
		n = me->query_skill("sword");
		victim->receive_damage("qi", n , me);
		victim->receive_wound("qi", n / 3, me);
		victim->receive_damage("neili", n , me);

		return     HIG "碧玉剑激射出一道白芒，闪电般攻向$n空门处的大穴。\n" NOR;


	}
	return damage_bonus;
}

