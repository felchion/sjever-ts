// ci.c canhe-zhi perform 刺字诀
// by leontt 1/5/2000
// modified by darken to reduce busy time

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;
int query_ap() {return 3;}
void remove_effect(object me, int improve);

int perform(object me, object target)
{
	string msg;
	int improve, lv;

	if( !target ) target = offensive_target(me);

  if( !objectp(target) || !target->is_character() || !me->is_fighting(target) || !living(target))
		return notify_fail("「刺字诀」只能在战斗中对对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
		return notify_fail("你必须空手使用「刺字诀」！\n");

	if( (int)me->query_skill("canhe-zhi",1) < 100 )
		return notify_fail("你的参合指不够娴熟，不会使用「刺字诀」！\n");

	if( (int)me->query_skill("shenyuan-gong",1) < 100 )
		return notify_fail("你的神元功等级不够，不能使用「刺字诀」！\n");

	if( (int)me->query("max_neili") < 1000 )
		return notify_fail("你的内力太弱，不能使用「刺字诀」！\n");

	if( (int)me->query("neili") < 600 )
		return notify_fail("你的内力太少了，无法使用出「刺字诀」！\n");

	if (me->query_skill_prepared("finger") != "canhe-zhi"
	|| me->query_skill_mapped("finger") != "canhe-zhi")
		return notify_fail("你现在无法使用「刺字诀」进行攻击。\n");

  if ( me->query_skill_mapped("force") != "shenyuan-gong")
    return notify_fail("没有神元功支持如何使用「刺字诀」！\n");
	
	msg = HIC"\n$N突然一掌拍在自己的丹田上激荡全身的内力形成一股「"HIR"剑气"HIC"」，从右手拇指激射而出，刺向$n的肩部。\n" NOR;
	message_vision(msg, me, target);

//	pfm ci 中不出 aut pfm 的标记 by lane
	me->set_temp("mr_ci", 1);

	lv = me->query_skill("canhe-zhi", 1);
	improve = 100 + lv/10;
	
  me->add_temp("apply/pfm_attack", 180+lv/10);
  me->add_temp("apply/damage", 250+lv/2);
  me->add_temp("apply/pfm_damage", 150+lv/5);

	me->add("neili", -300-lv/2);
		
	if( COMBAT_D->do_attack(me, target, 0, 0, 1) && !target->query_temp("ci")) {
		msg = HIR"$n觉得曲池穴一酸，手臂似乎失去了知觉。\n"NOR;
		message_vision(msg, me, target);
		if (present(target,environment(me)) && me->is_fighting(target))
				COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
		if (present(target,environment(me)) && me->is_fighting(target))
				COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(3)?1:3);
		if ( lv > 150)
			if (present(target,environment(me)) && me->is_fighting(target))
				COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
		if ( lv > 200)
			if (present(target,environment(me)) && me->is_fighting(target))
				COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
		target->add_temp("apply/pfm_attack", -improve);
		target->add_temp("apply/pfm_parry", -improve);
		target->add_temp("apply/pfm_dodge", -improve);
		target->set_temp("ci", improve);
		target->add_condition("no_perform", 1+random(2));
		call_out("check_fight", 1, me, target ,improve, me->query_skill("finger")/30+5);
	}
	else {
		msg = CYN"$n纵身一跃，敏捷地躲过了$N的这一招，$P趁$p躲闪之际，顺势速攻几招！\n"NOR;
		message_vision(msg, me, target);
		if (present(target,environment(me)) && me->is_fighting(target))
			COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
		if (present(target,environment(me)) && me->is_fighting(target))
			COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(3)?1:3);
		if ( lv > 150)
			if (present(target,environment(me)) && me->is_fighting(target))
				COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
		if ( lv > 200)
			if (present(target,environment(me)) && me->is_fighting(target))
				COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);	
	}
 
  me->add_temp("apply/pfm_attack", -180-lv/10);
  me->add_temp("apply/damage", -250-lv/2);
  me->add_temp("apply/pfm_damage", -150-lv/5);
	me->start_busy(random(2));

	me->delete_temp("mr_ci");
	return 1;
}

void remove_effect(object target, int improve)
{
	if(!target) return;
	target->add_temp("apply/pfm_attack", improve);
	target->add_temp("apply/pfm_parry", improve);
	target->add_temp("apply/pfm_dodge", improve);
	target->delete_temp("ci");
	tell_object(target, HIW"\n你运气一周只觉得手臂慢慢恢复了知觉。\n"NOR);
}

void check_fight(object me, object target, int improve, int count)
{
	if(! target) return;
	if(! me
	|| ! me->is_fighting(target)
	|| me->query_skill_mapped("finger") != "canhe-zhi"
	|| me->query_skill_prepared("finger") != "canhe-zhi"
	|| me->query_temp("weapon")
	|| count < 0 ){
		remove_effect(target , improve);
		return;
	}
	call_out("check_fight", 1, me, target, improve, count -1);
}

string perform_name(){ return HIC"刺字诀"NOR; }

int help(object me)
{
        write(HIC"\n「刺字诀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 600 以上；
                最大内力 1000 以上；
                参合指等级 100 以上；
                神元功等级 100 以上；
                激发指法为参合指；
                备指法为参合指；
                且手无兵器。

HELP
        );
        return 1;
}
                                
