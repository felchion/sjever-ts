// 观日剑

#include <weapon.h>
#include <ansi.h>

inherit SWORD;
inherit F_UNIQUE;

void create()
{
	set_name(HIR "观日剑" NOR, ({ "guanri jian", "guanri", "jian" }));
	set_weight(20000);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("treasure", 1);
		set("unit", "把");
		set("long", HIR "观日剑据说在观日峰十年铸成，光芒犹如初升之红日，能破魔消灾。\n" NOR);
		set("value", 10000);
		set("material", "steel");
		set("rigidity", 3);
		set("sharpness", 3);
		set("unique", 1);
		set("weapon_prop/parry", 2);
		set("wield_neili", 500);
		set("wield_maxneili", 900);
		set("wield_str", 23);
		set("weapon_prop/ap", 80);
		set("wield_msg", HIR "$N出手拔剑一转，一轮红日若从剑鞘中升起，剑光闪烁耀眼。\n" NOR);
		set("unwield_msg", HIR "只见$N手指微动，红日没于剑鞘之中。\n" NOR);
	}
	init_sword(110);
	setup();
}
mixed hit_ob(object me, object victim, int damage_bonus)
{
	int n;

	if (me->query_skill_mapped("sword") != "songshan-jian" ||
	me->query_skill("songshan-jian", 1) < 100)
	return damage_bonus / 2;

	switch (random(6))
	{
		case 0:
		if (! victim->is_busy())
		victim->start_busy(me->query_skill("sword") / 300 + 1);
		return HIR "观日剑幻化做一道道彩虹绕向$n,$n只觉手脚受束难以动弹。\n"NOR;



		case 1:
		n = me->query_skill("sword");
		victim->receive_damage("qi", n , me);
		victim->receive_wound("qi", n /3 , me);
		victim->receive_damage("neili", n , me);

		return HIY"观日剑幻化出一道耀眼金芒，周围空气中发出呲呲的声响！$n浑身一震，如受电击。\n" NOR;


	}
	return damage_bonus;
}
