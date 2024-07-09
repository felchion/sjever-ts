// jue.c 绝命针

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 3;}

string perform_name(){ return RED"绝命针"NOR; }
int perform(object me, object target)
{
	int lvl;
	object weapon = me->query_temp("weapon");

	if( !target ) target = offensive_target(me);

	if( !target
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("你只能对战斗中的对手使用。\n");

	if( (lvl = (int)me->query_skill("pixie-jian", 1)) < 140 )
	return notify_fail("你的辟邪剑法不够娴熟，不会使用绝命针。\n");

	if( (int)me->query_dex() < 35  )
	return notify_fail("你的身法太低，不能使用绝命针。\n");

	if( (int)me->query("neili", 1) < 1000 )
	return notify_fail("你现在内力不足！\n");


	if (!weapon || !(weapon->id("needle"))
	|| me->query_skill_mapped("sword") != "pixie-jian")
	return notify_fail("你现在无法使用绝命针。\n");

  me->set_temp("apply/action/pfm", RED"$N猛地纵起，向$n扑去，手指一弹，细针飞了出去，插向$n右目");
	target->set_temp("apply/action/dodge", HIY"\n可是$n看破了$N的企图，飞身闪了开去。\n"NOR);
	target->set_temp("apply/action/parry", HIY"\n可是$n看破了$N的企图，飞身闪了开去。\n"NOR);

	me->receive_damage("neili", 500+lvl/2);
	me->add_temp("apply/damage",400+lvl);
	me->add_temp("apply/pfm_damage",200+lvl/4);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	COMBAT_D->do_attack(me, target,weapon,3);
	me->add_temp("apply/damage",-400-lvl);
	me->add_temp("apply/pfm_damage",-200-lvl/4);
	me->add_temp("apply/pfm_attack",-180-lvl/10);
	me->delete_temp("apply/action/");

	me->start_busy(random(2));

	return 1;
}

int help(object me)
{
	write(HIW"\n辟邪剑法之「绝命针」："NOR"\n\n");
	write(@HELP
	
	要求：
		当前内力 1000 以上；
		辟邪剑法等级 140 以上；
HELP
	);
	return 1;
}