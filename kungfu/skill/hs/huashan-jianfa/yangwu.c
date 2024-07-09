// yangwu.c 养吾剑法

#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return HIW"养吾剑"NOR;}
int query_ap() {return 3;}

int perform(object me, object target)
{
	string msg;
	object weapon = me->query_temp("weapon");

	if( !target ) target = offensive_target(me);

	if( !target
	||!target->is_character()
	||!me->is_fighting(target) )
	return notify_fail("牵制攻击只能对战斗中的对手使用。\n");

	if((int)me->query_skill("zixia-gong", 1) < 110 )
	return notify_fail("你的紫霞神功内功等级不够，不能使用「养吾剑法」诀。\n");

	if ((int)me->query_skill("huashan-jianfa", 1) < 110 )
	return notify_fail("你的华山剑法不够娴熟，不会使用「养吾剑法」诀。\n");

	if (!weapon
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "huashan-jianfa"
	|| me->query_skill_mapped("parry") != "huashan-jianfa")
	return notify_fail("你现在无法使用牵制攻击。\n");

	if( (int)me->query("neili") < 500 )
	return notify_fail("你现在内力太弱，不能使用「养吾剑法」诀。\n");

	if( target->is_busy())
	return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧！\n");
	msg = MAG "$N招式一变展开华山「养吾剑法」，连挥数剑，立时剑气漫空，将$n逼的透不过气来。\n";

	me->add("neili", -500);
	if(COMBAT_D->do_attack(me, target, weapon, 0, 1) 
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)){
		msg += "结果$p被$P攻了个措手不及！\n" NOR;
		target->start_busy(6);
	}
	else {
		msg += "千钧一发之际,$p奋起神威也以醇厚内力向$P攻去，$P的养吾剑气立时无效只得撤剑变招。\n" NOR;
		target->add("neili",-600);
	}
	message_vision(msg, me, target);
	return 1;
}


int help(object me)
{
   write(WHT"\n华山剑法之「养吾剑」："NOR"\n");
   write(@HELP

    要求：紫霞神功 110级
          华山剑法 110级，
          当前内力 500以上。
HELP
   );
   return 1;
}

