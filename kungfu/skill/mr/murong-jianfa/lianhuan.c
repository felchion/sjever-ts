// lianhuan.c for murong-jianfa
// by snowman@SJ 01/05/1999.
// modify by leontt@SJ 26/09/2000.

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 5;}

int perform(object me,object target)
{
	object weapon;
	int i=me->query_skill("murong-jianfa",1);

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) || !living(target)
	|| environment(target)!=environment(me))
		return notify_fail("七剑连环指只能对战斗中的对手使用。\n");

	if((int)me->query_temp("mrjf/lianhuan"))
		return notify_fail("你正在使用七剑连环指！\n");
		
	if(me->query_skill_mapped("force") != "shenyuan-gong")
		return notify_fail("你的内功不是神元功，无法使用七剑连环指！\n");

	if( (int)me->query_skill("shenyuan-gong", 1) < 100 )
		return notify_fail("你的内功还未练成，不能使用七剑连环指！\n");

	if( (int)me->query_skill("murong-jianfa", 1) < 100 )
		return notify_fail("你的剑法还未练成，不能使用七剑连环指！\n");

	if(!objectp(weapon = me->query_temp("weapon"))
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "murong-jianfa")
		return notify_fail("你手里没有剑，无法使用七剑连环指！\n");

	if(me->query_skill_mapped("parry") != "murong-jianfa"
		&& me->query_skill_mapped("parry") != "douzhuan-xingyi")
		return notify_fail("你的招架功夫不对，无法使用七剑连环指！\n");
	if( (int)me->query_skill("canhe-zhi", 1) < 100 )
		return notify_fail("你的参合指还未练成，不能使用七剑连环指！\n");
	if((int)me->query("max_neili") < 1000 )
		return notify_fail("你的内力修为不够，不能使用七剑连环指！\n");
	if((int)me->query("neili") < 800 )
		return notify_fail("你现在真气不足，不能使用七剑连环指！\n");
	if((int)me->query("jingli") < 500 )
		return notify_fail("你现在精力不足，不能使用七剑连环指！\n");
	if(me->query_skill_mapped("finger") != "canhe-zhi"
	 || me->query_skill_prepared("finger") != "canhe-zhi")
		return notify_fail("你没有用指，无法使用七剑连环指！\n");
	if((int)me->query_skill("sword", 1) < 100 )
		return notify_fail("你的基本剑法不够娴熟，不能在剑招中使用七剑连环指。\n");

	message_vision(MAG"\n$N"MAG"使出慕容剑法之精髓「七剑连环指」，但见$P右手食指在"+weapon->name()+MAG"上一弹，画剑花朵朵相辉相映，直袭$n"MAG"！\n"NOR, me,target);
	me->set_temp("mrjf/lianhuan", 7);
	me->receive_damage("neili", 200);
	me->receive_damage("jingli", 200);
	me->add_temp("apply/pfm_attack", 210+i/10);
	me->add_temp("apply/pfm_damage", 200+i/4);
	me->add_temp("apply/damage", 200+i*2);
  me->set_temp("apply/action/color",HIC);
  me->set_temp("apply/action/skill","canhe-zhi");
  
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);

	call_out("check_fight", 2, me, 4, i);
	return 1;
}

void remove_effect(object me, int i)
{
	if(!me) return;

	me->delete_temp("mrjf/lianhuan");
	me->add_temp("apply/pfm_attack", -210-i/10);
	me->add_temp("apply/pfm_damage", -200-i/4);
	me->add_temp("apply/damage", -200-i*2);
	me->delete_temp("apply/action/");
	
	if (living(me)){
		message_vision(HIY"\n$N"HIY"的「七剑连环指」绝技运行完毕，气回丹田，缓缓收功。\n"NOR, me);
		i = 1 + random(2);
		me->add_busy(i);
	}
}

void check_fight(object me, int i, int j)
{
	object weapon, target;

	if (!me ) return;

	target = offensive_target(me);

	if(!i
	|| !living(me)
	|| !target
	|| !me->is_fighting(target)
	|| environment(target)!=environment(me)
	|| !me->query_temp("mrjf/lianhuan")
	|| me->query("neili") < 200
	|| me->query("jingli") < 200)
		return remove_effect(me, j);

	if(me->query_skill_mapped("sword") != "murong-jianfa") {
		tell_object(me, HIR"你临时改换剑法，决定不再使用「七剑连环指」绝技。\n");
		return remove_effect(me, j);
	} 
	else if(me->query_skill_mapped("force") != "shenyuan-gong") {
		tell_object(me, HIR"你临时改换内功，决定不再使用「七剑连环指」绝技。\n");
		return remove_effect(me, j);
	} 
	else if(me->query_skill_mapped("parry") != "murong-jianfa" 
		&& me->query_skill_mapped("parry") != "douzhuan-xingyi"){
		tell_object(me, HIR"你临时改换招架，决定不再使用「七剑连环指」绝技。\n");
		return remove_effect(me, j);
	} 
	else if(!objectp(weapon = me->query_temp("weapon"))
		|| weapon->query("skill_type") != "sword"){
		tell_object(me, HIR"你临时改换武器，决定不再使用「七剑连环指」绝技。\n");
		return remove_effect(me, j);
	} 
	else if (me->is_busy())
		call_out("check_fight", 1, me, i, j);
	else {
		me->receive_damage("neili", 150);
		me->receive_damage("jingli", 150);
		if (objectp(target) && me->is_fighting(target))
			COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
		call_out("check_fight", 2, me, --i, j);
	}
}

string perform_name(){ return MAG"七剑连环指"NOR; }

int help(object me) {
        write(MAG"\n「七剑连环指」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 800 以上；
                最大内力 1000 以上；
                当前精力 500 以上；
                慕容剑法等级 100 以上；
                神元功等级 100 以上；
                参合指等级 100 以上；
                激发剑法为慕容剑法；
                激发招架为慕容剑法或斗转星移；
                激发指法为参合指；
                激发内功为神元功。
HELP
        );
        return 1;
}