// chan 
// 99.3.17  by emnil

#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return HIY"金蛇缠丝"NOR;}

void do_chan(object me, object target, object weapon, int times);

int perform(object me, object target)
{
	string msg;
	object weapon;

	if( !target ) target = offensive_target(me);
	weapon = me->query_temp("weapon");

	if (!weapon
	|| weapon->query("skill_type") != "whip"
	|| me->query_skill_mapped("whip") != "lingshe-bianfa")
	return notify_fail("你手里没有鞭，无法使用「金蛇缠丝」！\n");

	if( !target
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("「金蛇缠丝」只能对战斗中的对手使用。\n");

	if( target->is_busy() )
	return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧！\n");

	if((int)me->query_skill("lingshe-bianfa", 1) < 100 )
	return notify_fail("你的灵蛇鞭法不够娴熟，不会使用「金蛇缠丝」绝技。\n");

	if((int)me->query_skill("dulong-dafa", 1) < 100 )
	return notify_fail("你的毒龙大法不够深厚，不会使用「金蛇缠丝」。\n");

	if((int)me->query("neili") < 500)
	return notify_fail("你现在内力太弱，不能使用金蛇缠丝。\n");

	if( me->query_skill_mapped("force") != "dulong-dafa")
	return notify_fail("你现在使用的内功与毒龙大法相抵触，不能使用「金蛇缠丝」。\n");

	me->add("neili", -500);
	me->add("jingli", -100);

	msg = HIY+"\n$N"+HIY+"突然以回旋劲摔出手中的"+weapon->query("name")+HIY+"，企图缠住$n的脖子！\n"NOR;


	if (COMBAT_D->do_attack(me, target, 0, 0, 1))	{
		msg +=HIR+"$n一个没留神，被"+weapon->query("name")+HIR+"缠个正着，一时之间气息不畅！\n"NOR;
		target->add_condition("no_exert", 1+random(3));
		target->add_condition("no_perform",1+random(3));
		me->start_busy(random(2));
	}
	else {
		msg += CYN+"可是$n看破了$N的企图，逃了过去。\n"NOR;
		me->start_busy(1);
	}
	
	message_vision(msg,me,target);
	
	return 1;
}


int help(object me)
{
        write(HIY"\n灵蛇鞭法之「金蛇缠丝」："NOR"\n");
        write(@HELP
        
        要求：最大内力 1000 以上；
              当前内力  500 以上；
              灵蛇鞭法等级 100 以上；                
              毒龙大法等级 100 以上；
              灵蛇鞭法为鞭法；
              手中持鞭；                
HELP
        );
        return 1;
}