// xingyi.c 斗转星移perform星移
//COOL@SJ,20000909

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 2;}
string perform_name(){ return HIC"星移斗转"NOR; }

int perform(object me, object target)
{
	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("星移斗转之星移只能对战斗中的对手使用。\n");

	if( (int)me->query_skill("douzhuan-xingyi", 1) < 120 )
	return notify_fail("你的星移斗转还未练成，不能使用！\n");

	if( (int)me->query("max_neili") < 1800 )
	return notify_fail("你现在内力修为不够，不能使用星移斗转之星移！\n");

	if( (int)me->query("neili") < 800 )
	return notify_fail("你现在真气不足，不能使用星移斗转之星移！\n");

	if( (int)me->query_skill("parry", 1) < 120 )
	return notify_fail("你的基本招架之法不够娴熟，不能使用星移斗转之星移。\n");

	if (me->query_skill_mapped("parry") != "douzhuan-xingyi")
	return notify_fail("你现在无法使用星移斗转。\n");

	if (!target->query_skill_prepare() && !target->query_temp("weapon"))
	return notify_fail("对方无招无式你怎么使用星移斗转。\n");

	call_out("xingyi_hit", 1, me, target);

	return 1;
}

int xingyi_hit(object me, object target)
{
	int skill,i;
	string msg;
	mapping apply;

	skill = (int)me->query_skill("douzhuan-xingyi", 1);

	target = offensive_target(me);
	if( !target
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target)
	|| environment(target) != environment(me) ){
		write("星移斗转只能对战斗中的对手使用。\n");
		return 0;
	}

	if (!target->query_skill_prepare() && !target->query_temp("weapon"))
	return notify_fail("你正要使用星移斗转，却发现对方无招无式。\n");

	if(!living(target))
	return notify_fail("你正要使用星移斗转，对手已经不能再战斗了。\n");

	if((int)me->query("neili", 1) < 500 )
	return notify_fail("你正要催力再击出一招，却发现自己的内力不够了！\n");

	if(me->query_skill_mapped("parry") != "douzhuan-xingyi")
	return notify_fail("你现在无法使用星移斗转。\n");

	me->add("neili", 500);

	apply=target->query_temp("apply");
	if (apply) {
		me->set_temp("douzhuan/damage_apply",(apply["damage"]));
		me->set_temp("douzhuan/attack_apply",(apply["pfm_attack"]));
		me->set_temp("douzhuan/pfm_damage_apply",(apply["pfm_damage"]));
	}

	msg = HIC "\n只见$N运招有若行云流水，借势把$n招式尽数反攻回去！\n"NOR;
	message_vision(msg, me, target);

	me->add_temp("apply/damage",     skill+me->query_temp("douzhuan/damage_apply"));
	me->add_temp("apply/pfm_attack", skill/5+me->query_temp("douzhuan/attack_apply"));
	me->add_temp("apply/pfm_damage", skill/4+me->query_temp("douzhuan/pfm_damage_apply"));

	for (i=0;i< 3;i++) {
		if(me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(3)?1:3);
	}

	me->add_temp("apply/damage",     -skill-me->query_temp("douzhuan/damage_apply"));
	me->add_temp("apply/pfm_attack", -skill/5-me->query_temp("douzhuan/attack_apply"));
	me->add_temp("apply/pfm_damage", -skill/4-me->query_temp("douzhuan/pfm_damage_apply"));
	me->delete_temp("douzhuan");
	me->start_busy(random(2));

	msg = HIW "$N运招完毕，嘴角掠过一丝轻鄙的微笑！\n"NOR;
	message_vision(msg, me, target);

	return 1;
}

int help(object me)
{
        write(HIC"\n「星移斗转」："NOR"\n");
        write(@HELP

        把对方的技能和武器的效果转移到自己身上，发出更强的攻击。
        
        要求：  最大内力 1800 以上；      
                当前内力 800  以上；  
                斗转星移等级 120 以上；
                基本招架等级 120 以上；
                激发招架为斗转星移。                
                
HELP
        );
        return 1;
}