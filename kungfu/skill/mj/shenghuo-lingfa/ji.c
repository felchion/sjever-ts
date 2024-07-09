// ji.c 圣火令法--击
// By River@SJ
// Modified by snowman@SJ 26/04/2001
// bug bug bug fix by lane@SJ 看来益处的 pfm 不单这些啊

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

int query_ap() {return 2;}
int checking(object me, object target);
int perform(object me, object target)
{
	int skill;
	object weapon;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("你只能对战斗中的对手使用「击」字诀。\n");

	if((int)me->query("neili") < 1000 )
	return notify_fail("你的内力不够。\n");

	if((int)me->query("jingli") < 800 )
	return notify_fail("你的精力不够。\n");

	if((int)me->query_temp("shlf_ji"))
	return notify_fail("你已经用双令的双击来扰乱对方了。\n");

	skill = (int)me->query_skill("shenghuo-lingfa", 1);

	if(skill < 120)
	return notify_fail("你的圣火令法等级还不够。\n");

	if (!me->query_skill("jiuyang-shengong"))
	if(me->query_skill("shenghuo-shengong", 1) < 120)
	return notify_fail("你的圣火神功等级还不够。\n");

	if(! weapon=me->query_temp("weapon"))
	return notify_fail("你不用兵器怎么来双击？\n");

	if( weapon->query_amount() < 2 && weapon->query("dagger_count") < 2 )
	return notify_fail("你只用一把令牌，怎么双击？\n");

	if ( me->query_skill_mapped("dagger") != "shenghuo-lingfa"
	|| me->query_skill_mapped("cuff") != "shenghuo-lingfa"
	|| me->query_skill_prepared("cuff") != "shenghuo-lingfa"
	&& userp(me))
	return notify_fail("你必须先将圣火令法功结合使用。\n");

	if( me->query_skill_mapped("parry") != "shenghuo-lingfa"
	&& me->query_skill_mapped("parry") != "qiankun-danuoyi")
	return notify_fail("你需要激发招架为圣火令法，方能使用此绝技。\n");


	me->add("neili", -800);
	message_vision(HIY"\n$N飞身上前，双手将两块"+weapon->name()+HIY"相互一击，铮的一声，声音非金非玉，十分古怪。\n"NOR,me);
	me->set_temp("shlf_ji", 1);
	checking(me,target);
	return 1;
}

int check_fight(object me, object target)
{
	object weapon;
	int skill;
	if (!me ||!me->query_temp("shlf_ji") || !living(me)){
		if( target) {
			skill= target->query_temp("ji");
			target->add_temp("apply/pfm_attack", 120);
			target->add_temp("apply/pfm_parry", 120);
			target->add_temp("apply/pfm_dodge", 120);
			target->delete_temp("ji");
			message_vision(HIW"\n$n耳边的噪音渐渐失去，神志又恢复了自然。\n"NOR,me,target);
			return 1;
		}
		return 1;
	}
	if (!target){
		me->delete_temp("shlf_ji");
		return 1;
	}
	me->delete_temp("shlf_ji");
	skill = me->query_skill("shenghuo-lingfa", 1);
	if(!weapon = me->query_temp("weapon")){
		if(target->is_busy()) target->start_busy(1);
		target->add_temp("apply/pfm_attack", 120);
		target->add_temp("apply/pfm_parry", 120);
		target->add_temp("apply/pfm_dodge", 120);
		message_vision(HIW"\n$N放下手中令牌，$n耳边的噪音失去，神志又恢复了自然。\n"NOR,me,target);
	}
	else if(environment(me) != environment(target)){
		if(target->is_busy()) target->start_busy(1);
		target->add_temp("apply/pfm_attack", 120);
		target->add_temp("apply/pfm_parry", 120);
		target->add_temp("apply/pfm_dodge", 120);
		message_vision(HIW"\n$N离开$n了，$n耳边的噪音失去，神志又恢复了自然。\n"NOR,me,target);
	}
	else if(!target->is_busy()){
		target->add_temp("apply/pfm_attack", 120);
		target->add_temp("apply/pfm_parry", 120);
		target->add_temp("apply/pfm_dodge", 120);
		message_vision(HIW"\n$n耳边的噪音渐渐失去，神志又恢复了自然。\n"NOR,me,target);
	}
	else if(!me->is_fighting(target)){
		if(target->is_busy()) target->start_busy(1);
		target->add_temp("apply/pfm_attack", 120);
		target->add_temp("apply/pfm_parry", 120);
		target->add_temp("apply/pfm_dodge", 120);
		message_vision(HIW"\n$N跳开战圈，$n耳边的噪音失去，神志又恢复了自然。\n"NOR,me,target);
	}
	else if (userp(me) && me->query_skill_mapped("dagger") != "shenghuo-lingfa"
	|| me->query_skill_mapped("cuff") != "shenghuo-lingfa"){
		if(target->is_busy()) target->start_busy(1);
		me->start_busy(1);
		target->add_temp("apply/pfm_attack", 120);
		target->add_temp("apply/pfm_parry", 120);
		target->add_temp("apply/pfm_dodge", 120);
		message_vision(HIW"\n$N不再用圣火令法攻击，$n耳边的噪音失去，神志又恢复了自然。\n"NOR,me,target);
	}
	else {
		me->set_temp("shlf_ji", 1);
		call_out("check_fight", 1, me, target, weapon);
		return 1;
	}
	return 1;
}

int checking(object me, object target)
{
	int skill;
	object weapon = me->query_temp("weapon");

	skill = (int)me->query_skill("shenghuo-lingfa", 1);
	if (COMBAT_D->do_attack(me, target, weapon, 0, 1)
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1) ){
		message_vision(HIR"只听见铮的一响，$n心神一荡，身子从半空中直堕下来。\n"NOR,me,target);
		target->add_busy(2+random(3));
		target->set_temp("ji",skill);
		target->add_temp("apply/pfm_attack", -120);
		target->add_temp("apply/pfm_parry", -120);
		target->add_temp("apply/pfm_dodge", -120);
		me->set_temp("shlf_ji", 1);
		me->start_busy(random(2));
		call_out("check_fight", 1, me, target);
	}
	else {
		me->start_busy(random(2));
		message_vision(CYN"$n心神一震，但随即恢复了神志，没被令牌双击之声所迷惑。\n"NOR,me,target);
		me->delete_temp("shlf_ji");
	}
	return 1;
}

string perform_name(){ return HIY"击字诀"NOR; }

int help(object me)
{
	write(HIY"\n圣火令法之「击字诀」："NOR"\n\n");
	write(@HELP
	要求：	
		当前内力 1000 以上；
		圣火令法等级 120 以上；
		圣火神功等级 120 以上；
		激发令法为圣火令法；
		激发拳法为圣火令法；
		激发招架为圣火令法或乾坤大挪移；
		备拳法为圣火令法；
		且手持两把兵器。

HELP
        );
        return 1;
}
