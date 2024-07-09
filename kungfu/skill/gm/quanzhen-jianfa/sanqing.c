// by darken@SJ
// Modify By River@SJ 2001.6.18

#include <ansi.h>

inherit F_DBASE;
inherit F_SSERVER;
int query_ap() {return 5;}

string perform_name(){ return HIW"一剑化三清"NOR; }
int perform(object me, object target)
{
	int lv = me->query_skill("quanzhen-jianfa", 1);
	object weapon = me->query_temp("weapon");
	string msg;
	
	if (me->query("gender") == "女性") lv = lv/2;
	
	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
		return notify_fail("一剑化三清只能对战斗中的对手使用。\n");

	if( me->query_skill("yunu-xinjing", 1) < 100 )
		return notify_fail("你剑法火候未足，无法使用一剑化三清！\n");

	if( me->query("neili") < 300+lv/2 )
		return notify_fail("你的内力不够使用一剑化三清！\n");
	
	if( me->query("max_neili") < 1500 )
		return notify_fail("你的内力修为太低，不能使用一剑化三清！\n");
		
	if((int)me->query_temp("hebi"))
		return notify_fail("你正在施展双剑合壁，无法使用「一剑化三清」绝技。\n");
	
	if((int)me->query_temp("gumu_suxin"))
		return notify_fail("你现在正在使用「玉女素心」绝技。\n");

	if((int)me->query_temp("gm/sanqing"))
		return notify_fail("你现在正在使用「一气化三清」绝技。\n");
	
	if(me->query_skill("yunu-xinjing", 1) < 100)
		return notify_fail("你的内功火候不够，无法使用一剑化三清。\n");

	if (!weapon
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "quanzhen-jianfa")
		return notify_fail("你手中无剑怎能使用一剑化三清？！\n");

	msg = HIW"\n$N一声清啸，使出全真剑法中的一剑化三清，剑尖分成数点，刺向$n！\n" NOR;
	message_vision(msg , me, target);
	me->add_temp("apply/pfm_attack", 150+lv/10);
	me->add_temp("apply/damage", 350+lv/2);
	me->add_temp("apply/pfm_damage", 140+lv/5);	
	me->add("neili", -250-lv/2);
	me->set_temp("gm/sanqing", 1);
	me->set_temp("sanqing", 1);
	me->set_temp("apply/action/color",HIW);
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
	if (me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(3)?1:3);
	if (me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), (random(2)+lv/400)?3:1);	
	me->delete_temp("apply/action/color");
	me->delete_temp("sanqing");
	call_out("check_fight", 8, me, target, weapon, lv, lv/20);
	return 1;
}

void check_fight(object me,object target,object weapon,int lv,int count)
{
	if(!me) return;
	if(!objectp(target)
	|| !me->is_fighting(target)
	|| !weapon
	|| me->query_skill_mapped("sword") != "quanzhen-jianfa"
	|| me->query("neili") < 500+lv/2
	|| count < 3 ){
		if (me->is_fighting(target))
			message_vision(HIW"\n这几剑一气呵成，剑意连环，剑势沉稳，取意道家三清，乃是全真剑法中的精髓所在。\n\n"NOR, me);
		me->delete_temp("gm/sanqing");
	me->add_temp("apply/pfm_attack", -150-lv/10);
	me->add_temp("apply/damage", -350-lv/2);
	me->add_temp("apply/pfm_damage", -140-lv/5);	
		if (target) 
		  me->start_busy(2+random(2));
		return;
	}
	else {
		me->set_temp("sanqing", 1);
		me->set_temp("apply/action/color",HIW);
		me->add("neili", -250-lv/2);
		if (me->is_fighting(target))
			COMBAT_D->do_attack(me, target, me->query_temp("weapon"),1);
		if (me->is_fighting(target))
			COMBAT_D->do_attack(me, target, me->query_temp("weapon"),random(3)?1:3);
		if (me->is_fighting(target))
			COMBAT_D->do_attack(me, target, me->query_temp("weapon"),(random(2)+lv/300)?3:1);	
		me->delete_temp("sanqing");
		me->set_delete("apply/action/color");
	}
	call_out("check_fight", 8, me, target, weapon, lv,count - 3);
}

int help(object me)
{
        write(HIW"\n全真剑法之「一剑化三清」："NOR"\n\n");
        write(@HELP
        要求：  当前内力  800 以上；
                最大内力 1500 以上；
                全真剑法等级 100 以上；
                玉女心经等级 100 以上；
                激发剑法为全真剑法；

HELP
        );
        return 1;
}    