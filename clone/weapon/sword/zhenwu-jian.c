// zhenwu-jian.c 真武剑
#include <ansi.h>
#include <weapon.h>

inherit SWORD;
inherit F_UNIQUE;

void create()
{
	set_name(HIW"真武剑"NOR, ({"zhenwu jian", "sword", "zhenwu"}));
	set_weight(20000);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "把");
		set("long", "一把毫不起眼的古剑,剑身不规则的花纹上似乎还带有些铁锈,剑锋上居然还有几个缺口。\n");
		set("value", 1);
		set("unique", 1);
		set("rigidity", 5);
		set("sharpness", 5);
		set("material", "blacksteel");
		set("weapon_prop/parry", 4);
		set("treasure",1);
		set("wield_neili", 900);
		set("wield_maxneili", 1400);
		set("wield_str", 24);
		set("weapon_prop/ap", 70);
		set("wield_msg",HIW"\n只听「唰」的一声，$N手里多了一把寒光四射的宝剑，正是武当山镇山之宝「真武剑」！\n"NOR);
		set("unwield_msg",HIW"$N把手中的真武剑插回剑鞘。\n"NOR);
	}
	init_sword(150);
	setup();
}

mixed hit_ob(object me, object victim, int damage_bonus)
{
	int n;

	if (me->query_skill_mapped("sword") != "taiji-jian" ||
	me->query_skill("taiji-jian", 1) < 100)
	return damage_bonus / 2;

	switch (random(8))
	{
		case 0:
		if (! victim->is_busy())
		victim->start_busy(random(me->query_skill("sword") / 300 )+ 1);
		return HIC "$N" HIC "跨前一步，手中的" NOR + HIY "真武剑" NOR
		+ HIC "幻化成无数圆圈，向$n" HIC "逼去，剑法细密之极。"
		"$n" HIC "大吃一\n惊，不知如何抵挡，只有连连后退！\n" NOR;


		case 1:
		n = me->query_skill("sword");
		victim->receive_damage("qi", n , me);
		victim->receive_wound("qi", n / 3, me);
		victim->receive_damage("neili", n , me);

		return             HIY "$N" HIY "突然大声喝道：“邪魔外道，还"
		"不受死？”手中真武剑" HIY "忽的一抖，$n"
		HIY "登时觉得眼花缭乱！\n" NOR;

	}
	return damage_bonus;
}

