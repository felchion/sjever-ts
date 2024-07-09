// pi.c 打狗棒法--劈字诀
// Creat by looklove@SJ 3/21/2001
// Mofify by looklove 2001.4.8
// Update by lsxk@hsbbs 2007/7/2

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

int query_ap() {return 4;}

string perform_name(){ return HBRED"劈字诀"NOR; }
int perform(object me, object target)
{
	int lvl;
	object weapon;

	weapon = me->query_temp("weapon");
	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target) )
		return notify_fail("「劈字诀」只能在战斗中使用。\n");

	if (!objectp(weapon)
	 || weapon->query("skill_type") != "stick"
	 || me->query_skill_mapped("stick") != "dagou-bang"
	 || me->query_skill_mapped("parry") != "dagou-bang" )
		return notify_fail("你手中无棒，无法使用「劈字诀」！\n");

   if( (int)me->query_skill("huntian-qigong", 1) < 200 )
		return notify_fail("你的混天气功修为不够，使不出「劈字诀」绝技。\n");

	if( me->query_skill_mapped("force") != "huntian-qigong" )
		return notify_fail("没有混天气功作为根基，是无法使出「劈字诀」绝技的！\n");

   if( lvl=(int)me->query_skill("dagou-bang",1) < 200 )
		return notify_fail("你的打狗棒法修为不够,目前还不能使用「劈字诀」！\n");

   if( (int)me->query_skill("bangjue",1) < 180 )
		return notify_fail("「劈字诀」是以打狗棒诀为基础的，你棒决不熟如何使用？\n");

   if( (int)me->query("max_neili") < 2500 )
		return notify_fail("你内力修为现在不够, 不能用「劈字诀」！\n");

	if( (int)me->query("neili") < 800 )
		return notify_fail("你内力用尽, 不能用「劈字诀」！\n");

	if( (int)me->query("jingli") < 600 )
		return notify_fail("你现在没有足够的精力使用「劈字诀」。\n");

	me->set_temp("apply/action/pfm",  HBRED"$N使出「劈字诀」，挥舞起手中"+weapon->name()+HBRED"如狂风暴雨向$n劈头盖脸得打去");
	me->set_temp("apply/action/hit",HBRED"\n$n只觉得眼花缭乱，一个不留神，被"+weapon->name()+HBRED"连连打中！\n"NOR);
	target->set_temp("apply/action/parry", HIC"\n$n大惊失色，慌乱中一个就地十八滚，堪堪避过$N的"+weapon->name()+HIC"。\n"NOR);
	target->set_temp("apply/action/dodge", HIC"\n$n大惊失色，慌乱中一个就地十八滚，堪堪避过$N的"+weapon->name()+HIC"。\n"NOR);
	
	me->receive_damage("neili", 300+lvl/2);
	
	me->add_temp("apply/damage",600+lvl);
	me->add_temp("apply/pfm_damage",300+lvl/3);
	me->add_temp("apply/pfm_attack",240+lvl/10);
	COMBAT_D->do_attack(me, target,weapon,random(2)?1:3);
	me->add_temp("apply/damage",-600-lvl);
	me->add_temp("apply/pfm_damage",-300-lvl/3);
	me->add_temp("apply/pfm_attack",-240-lvl/10);

	me->delete_temp("apply/action");
	target->delete_temp("apply/action");
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
	write(HBRED"\n打狗棒「劈字诀」："NOR"\n");
	write(@HELP
打狗棒中霸气十足的招数，当此莫属。此招出手，往往自己
也无法控制威力，必杀一击。当者披靡。

	要求：	
	最大内力 2500 以上；
	当前内力 800 以上；
	当前精力 600 以上；
	打狗棒等级 200 以上；
	混天气功等级 200 以上；
	打狗棒诀等级 180 以上：
	激发招架为打狗棒。
HELP
	);
	return 1;
}
