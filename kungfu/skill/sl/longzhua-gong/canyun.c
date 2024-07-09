// canyun 风卷残云

#include <ansi.h>
#include <combat.h> 
inherit F_SSERVER;

string perform_name(){ return HIW"风卷残云"NOR; }

int perform(object me, object target)
{
	int lev,i;
	lev = me->query_skill("longzhua-gong", 1);

	if( !objectp(target) ) target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) )
	return notify_fail("「风卷残云」只能在战斗中使用。\n");
	if( me->query_temp("weapon"))
	return notify_fail("你拿着武器怎么能使用「风卷残云」！\n");
	if( (int)me->query_skill("longzhua-gong", 1) < 120 )
	return notify_fail("你的龙爪功还不够娴熟，使不出「风卷残云」绝技。\n");
	if (me->query_skill_prepared("claw") != "longzhua-gong"
	|| me->query_skill_mapped("claw") != "longzhua-gong")
	return notify_fail("你现在无法使用「风卷残云」进行攻击。\n");

	if( (int)me->query_skill("yijin-jing", 1) < 120 )
	return notify_fail("你的易筋经等级还不够，使不出「风卷残云」绝技。\n");
	if( me->query("max_neili") < 1000 )
	return notify_fail("你现在内力太弱，使不出「风卷残云」。\n");
	if( (int)me->query("neili") < 500 )
	return notify_fail("你现在真气太弱，使不出「风卷残云」。\n");
	message_vision(RED"\n$N突然大喝一声，纵身而上，双手犹如狂风骤雨，使出「风卷残云」，漫天爪影带着气浪有如怒海狂滔一般！\n"NOR,me);

	me->add("neili", -300-lev/5);
	me->add_temp("apply/pfm_attack", 150+lev/10);
	me->add_temp("apply/damage", 150+lev/2);
	me->add_temp("apply/pfm_damage", 140+lev/5);

	for(i = 0; i < 2; i++)
	  if ( objectp(target) && me->is_fighting(target))
	    COMBAT_D->do_attack(me, target,0, random(2)?3:1);


	me->add_temp("apply/pfm_attack", -150-lev/10);
	me->add_temp("apply/damage", -150-lev/2);
	me->add_temp("apply/pfm_damage", -140-lev/5);
	me->start_busy(random(2));


	return 1;
}

int help(object me)
{
        write(HIW"\n「风卷残云」："NOR"\n");
        write(@HELP
        
        以狂风骤雨之势将龙爪功的狠辣之处发挥得淋漓尽致。与
        「连环八式」配合使用时威力倍增。

        要求：  最大内力 1000 以上；
                当前内力 500 以上；
                龙爪功等级 120 以上；
                易筋经等级 120 以上；
                激发龙爪功为爪法；
                
HELP
        );
        return 1;
}