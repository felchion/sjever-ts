// modified by snowman@SJ 05/12/2000

// Modify By River@Sj



#include <ansi.h>
inherit F_SSERVER;
#include <combat.h>
#include <combat_msg.h>

int query_ap() {return 3;}

int next1(object me, object target);
int next2(object me, object target);

int perform(object me,object target)
{
	object weapon;
	string msg;


	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !me->is_fighting(target)
	|| !living(target)
	|| environment(target)!=environment(me))
		return notify_fail("天女散花只能对战斗中的对手使用。\n");

	if(me->query_skill_mapped("force") != "shenyuan-gong")
		return notify_fail("你的内功不是神元功，无法使用天女散花！\n");

	if( me->query_skill("shenyuan-gong", 1) < 120 )
		return notify_fail("你的内功还未练成，不能使用天女散花！\n");

	if( me->query_skill("murong-jianfa", 1) < 120 )
		return notify_fail("你的剑法还未练成，不能使用天女散花！\n");

	if (!objectp(weapon = me->query_temp("weapon"))
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "murong-jianfa")
		return notify_fail("你手里没有剑，无法使用天女散花！\n");

	if(me->query_skill_mapped("parry") != "murong-jianfa"
	&& me->query_skill_mapped("parry") != "douzhuan-xingyi")
		return notify_fail("你的招架功夫不对，无法使用天女散花！\n");

	if(me->query("max_neili") < 1200 )
		return notify_fail("你的内力修为不够，不能使用天女散花！\n");

	if(me->query("neili") < 1000 )
		return notify_fail("你现在真气不足，不能使用天女散花！\n");

	if(me->query("jingli") < 600 )
		return notify_fail("你现在精力不足，不能使用天女散花！\n");

	if(me->query_temp("sanhua"))
		return notify_fail("你正在使用天女散花！\n");

	if(me->query_skill("sword", 1) < 120 )
		return notify_fail("你的基本剑法不够娴熟，不能在剑招中使用天女散花。\n");


	msg = HIC"\n$N使出「天女散花」，点点剑光直飞$n全身各大要穴。\n"NOR;

	if(COMBAT_D->do_attack(me, target, weapon,0, 1)
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)) {
		msg += HIW "$n被$N攻了个措手不及，一剑正刺中" + HIR"「檀中穴」"HIW + "。\n"NOR;
		target->add_temp("apply/pfm_attack", -120);
		target->add_temp("apply/pfm_dodge", -120);
		target->add_temp("apply/pfm_parry", -120);
		me->set_temp("sanhua", 1);
		me->add("neili", -150);
		call_out("back", 30, target);
	}
	else {
		me->add("neili", -80);
		msg += MAG "\n可是$n看破了$N的企图，并没有上当。\n" NOR;
		me->start_busy(random(2)+1);
	}

	message_vision(msg, me, target);

	if ( me->query_skill("murong-jianfa", 1) > 140)
		next1(me, target);
	else
		me->delete_temp("sanhua");
	return 1;

}



int next1(object me, object target) {
	int lvl=me->query_skill("murong-jianfa",1);
	object weapon;

	if (!me) return 0;
	weapon = me->query_temp("weapon");
	if (!weapon || !target || !living(me) || !me->is_fighting(target)) {
		me->delete_temp("sanhua");
		return 0;
	}

	me->set_temp("apply/action/pfm",HIW"紧跟着$N剑锋一转，欺身近前，从不可思议的角度刺向$n的" + HIR"「命门穴」"NOR);
  me->set_temp("apply/action/hit", CYN"$n只觉全身真气由「命门穴」狂泻而出。\n"NOR);       
	target->set_temp("apply/action/parry", CYN"\n$n一个懒驴打滚，危急之中躲过致命一击。\n"NOR);
	target->set_temp("apply/action/dodge", CYN"\n$n一个懒驴打滚，危急之中躲过致命一击。\n"NOR);	

	me->add_temp("apply/pfm_attack",240+lvl/10);
	me->add_temp("apply/pfm_force",150+lvl/5);
	me->add_temp("apply/force",300+lvl/2);
  COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1); 
	me->add_temp("apply/pfm_attack",-240-lvl/10);
	me->add_temp("apply/pfm_force",-150-lvl/5);
	me->add_temp("apply/force",-300-lvl/2);
	if (me->query_skill("murong-jianfa", 1) > 160) next2(me, target);
	else me->delete_temp("sanhua");
	return 1;

}

int next2(object me, object target){
	string msg;
	object weapon;

	if( !me ) return 0;

	weapon = me->query_temp("weapon");
	if (!weapon || !target || !living(me) || !me->is_fighting(target)) {
		me->delete_temp("sanhua");
		return 0;
	}

	msg = HIW"\n就在$n疲于防守之际，$N又风驰电掣地刺出第三剑。\n";


	if( COMBAT_D->do_attack(me, target, weapon,0, 1)
	||COMBAT_D->do_attack(me, target, weapon,0, 1)
	||COMBAT_D->do_attack(me, target, weapon,0,1) ) {
		msg += HIW"\n结果刺中$n的" + HIR"「环跳穴」"HIW + "。\n"NOR;
		message_vision(msg, me, target);
		target->add_busy(random(3)+2);
		me->add("neili",-150);
		me->add("jingli",-80);
	}
	else {
		msg += HIW "\n$n一个旱地拔葱，跃起数丈，堪堪避过。\n" NOR;
		message_vision(msg, me, target);
		me->add("neili", -70);
	}

	me->delete_temp("sanhua");
	me->delete_temp("sanhua_action");
	me->add_busy(random(2));
	return 1;
}


void back(object target) {
	if (!target) return;
	target->add_temp("apply/pfm_attack", 120);
	target->add_temp("apply/pfm_dodge", 120);
	target->add_temp("apply/pfm_parry", 120);
}


string perform_name(){ return HIG"天女散花"NOR; }

int help(object me) {
        write(HIG"\n「天女散花」："NOR"\n\n");

        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 1200 以上；
                当前精力 600 以上；
                慕容剑法等级 120 以上；
                神元功等级 120 以上；
                基本剑法等级 120 以上；
                激发剑法为慕容剑法；
                激发招架为慕容剑法或斗转星移；
                激发内功为神元功；
                与「七剑连环指」配合使用命中提高。
HELP
        );
        return 1;

}