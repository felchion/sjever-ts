// Created by LSXK@hsbbs 2007/5/10
// ci.c 苗家剑法之「绝命刺」.

#include <ansi.h>
#include "/kungfu/skill/eff_msg.h";

inherit F_SSERVER;

int query_ap() {return 10;}

string perform_name() {return HBBLU"绝命刺"NOR;}

int perform(object me, object target)
{
	object weapon;
	int damage;
	string msg,result;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	||  !target->is_character()
	||  !me->is_fighting(target)
	||  !living(target))
	return notify_fail("「绝命刺」只能对战斗中的对手使用。\n");

	if( (int)me->query_skill("miaojia-jianfa", 1) < 150 )
	return notify_fail("你的苗家剑法功力太浅，使不出苗家剑法之「绝命刺」的功夫!\n");

	if( (int)me->query_skill("sword", 1) < 150 )
	return notify_fail("你的基本剑法太差,使不出「绝命刺」的功夫!\n");

	if( (int)me->query("max_neili") < 800 )
	return notify_fail("你的内力修为太浅，使不出「绝命刺」。\n");

	if( (int)me->query("neili") < 400 )
	return notify_fail("你的真气不够，使不出「绝命刺」功夫。\n");

	if (!objectp(weapon = me->query_temp("weapon")) || weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "miaojia-jianfa")
	return notify_fail("你不使用苗家剑法,如何使得了「绝命刺」?\n");

	damage = me->query_skill("miaojia-jianfa", 1) * 30;
	damage += random(damage/4);
	msg = HBBLU +HIW "\n$N突然运足内力，只听手中的"+weapon->name()+HBBLU+HIW"「嗡」的一声清响，便以雷霆万钧之势刺向$n。\n"NOR;
	me->receive_damage("jingli",200+random(100));
	me->receive_damage("neili",400+random(200));
	if(COMBAT_D->do_attack(me, target, weapon, 0, 1) 
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1))
	{
		msg+=HIR"但听$N一声清喝：「着！」，"+weapon->name()+HIR"已经在$n胸口对穿而过，殷红的鲜血瞬间从$n胸口喷了出来，眼见是活不成了。\n"NOR;
		if(COMBAT_D->do_attack(me, target, weapon, 0, 1) || COMBAT_D->do_attack(me, target, weapon, 0, 1))
		{
			target->set("qi",-1);
			msg += RED"$n哪里见过如此狠辣的招数，双眼惊恐地看着$N,终于支持不住，倒下了。\n"NOR;
			message_vision(msg,me,target);
			COMBAT_D->report_status(target,1);
			me->start_busy(2);
			return 1;
		}
		else
		{
			target->receive_damage("qi",damage,me);
			target->receive_wound("qi",damage/2,me);
			result = damage_msg(damage,"刺伤");
			result = replace_string( result, "$l", "胸口" );
			result = replace_string( result, "$w", weapon->name() );
			msg += result;
			message_vision(msg,me,target);
			COMBAT_D->report_status(target,1);
			me->start_busy(1);
			return 1;
		}
	}
	else {
		msg += HIG"\n$n见这一剑来势如此之猛，哪敢正面接招，连忙运足轻功，往旁边一跃，远远的躲了开去。\n"NOR;
		message_vision(msg,me,target);
	}
	return 1;

}

int help(object me)
{
   write(WHT"\n苗家剑法「"HBBLU+HIW"绝命刺"NOR+WHT"」："NOR"\n");
   write(@HELP
   「绝命刺」，光听这招名字，便让人感到心惊胆战。招试异常朴实，但
   对手若是被该招击中，哪怕便要真的应了这招的名字！

   要求：  内力 400 以上；
           最大内力 800 以上； 
           苗家剑法 150 以上；
           基本剑法 150 以上；

HELP
   );
   return 1;
}
