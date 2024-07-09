// wofo.c yizhi-chan perform 皈依我佛

inherit F_SSERVER;

#include <ansi.h>
#include <combat.h>


string perform_name(){ return HIY"皈依我佛"NOR; }

int perform(object me, object target)
{
	string msg;
	int busy,lvl;

	if( !target ) target = offensive_target(me);

	if( !target || !me->is_fighting(target) )
	return notify_fail("「皈依我佛」只能在战斗中对对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
	return notify_fail("你必须空手使用「皈依我佛」！\n");

	if( (int)me->query_skill("yizhi-chan",1) < 100 )
	return notify_fail("你的一指禅不够娴熟，不会使用「皈依我佛」！\n");

	if( (int)me->query_skill("yijin-jing",1) < 100 )
	return notify_fail("你的易筋经内功等级不够，不能使用「皈依我佛」！\n");

	if( (int)me->query("max_neili") < 1000 )
	return notify_fail("你的内力太弱，不能使用「皈依我佛」！\n");

	if( (int)me->query("neili") < 500 )
	return notify_fail("你的内力太少了，无法使用出「皈依我佛」！\n");

	if (me->query_skill_prepared("finger") != "yizhi-chan"
	|| me->query_skill_mapped("finger") != "yizhi-chan")
	return notify_fail("你现在无法使用「皈依我佛」进行攻击。\n");

	if (target->is_busy()) return notify_fail("对方正自顾不暇，放胆攻击吧！\n");

	msg = HIY"$N突然身行纵向半空，右手屈指弹出，嗤的一声，一股劲气激射而出，罩向$n周身大穴。\n" NOR;
	message_vision(msg, me, target);

	lvl=me->query_skill("yizhi-chan",1);
	busy=3+random(lvl)/100;

	if(busy>7) busy=7;

	if(COMBAT_D->do_attack(me, target, 0, 0, 1) 
	|| COMBAT_D->do_attack(me, target, 0, 0, 1)
	|| COMBAT_D->do_attack(me, target, 0, 0, 1)){
		msg = HIW"$n只感到穴道一麻，浑身劲气立散，动弹不得。\n"NOR;
		message_vision(msg, me, target);
		me->add("neili", -350);
		target->start_busy(busy);
	}
	else {
		msg = HIY"$n看出$N的企图，一闪身，敏捷地躲过了$N的这一招！\n"NOR;
		message_vision(msg, me, target);
		me->start_busy(random(2));
		me->add("neili", - 100);
	}
	return 1;
}



int help(object me)
{
        write(HIY"\n「皈依我佛」："NOR"\n");
        write(@HELP

        要求：  最大内力 1000 以上；
                当前内力 500 以上；
                一指禅等级 100 以上；
                易筋经等级 120 以上；
                激发一指禅为指法；
                手中不能持有武器；
                
HELP
        );
        return 1;
}