// Created by lsxk@hsbbs 2007/6/13

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;
int query_ap() {return 2;}

string perform_name() {return HIW"六脉神剑"NOR;}

int perform(object me, object target)
{
	int lev;


	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target) )
	return notify_fail("「六脉神剑」只能在战斗中使用。\n");

	if( me->query_temp("weapon"))
	return notify_fail("你拿着武器怎么能使用「六脉神剑」！\n");

	if( (int)me->query_skill("liumai-shenjian", 1) < 30 )
	return notify_fail("你还尚未精通「六脉神剑」之一脉，如何使得出该绝技?\n");

	if((int)me->query_skill("force") < 250 )
	return notify_fail("你的内功修为还不够，使不出「六脉神剑」绝技。\n");

	if( (int)me->query("jiali")< 50 )
	return notify_fail("你使用过少的内力伤敌，无法将「六脉神剑」的威力体现出来！\n");

	if( me->query_temp("pf_rong"))
	return notify_fail("你正在运用「荣」字大法！\n");

	if(!me->query_skill("kurong-changong", 1))
	if(me->query_skill("qiantian-yiyang", 1) < 250)
	return notify_fail("以你现在的内功修为还使不出「六脉神剑」。\n");

	if (me->query_skill_prepared("finger") != "yiyang-zhi"
	|| me->query_skill_mapped("finger") != "yiyang-zhi" )
	return notify_fail("你现在无法使用「六脉神剑」！\n");

	if (me->query_skill_mapped("force") != "kurong-changong" )
	if (me->query_skill_mapped("force") != "qiantian-yiyang")
	return notify_fail("你所使用的内功不对。\n");

	if( (int)me->query("max_neili") < 4500 )
	return notify_fail("你现在内力太弱，使不出「六脉神剑」。\n");

	if( (int)me->query("neili") < 2000)
	return notify_fail("你现在真气太弱，使不出「六脉神剑」。\n");

	if( (int)me->query("jingli") < 1000)
	return notify_fail("你现在精力太少，使不出「六脉神剑」。\n");

	if( me->query_temp("yyz/6mai/6mai"))
	return notify_fail("你正在使用「六脉神剑」呢。\n");

	lev = (int)me->query_skill("yiyang-zhi", 1);

	message_vision(HIW"\n$N深吸一口"HIY"纯阳真气"HIW"，内力运转于指尖，瞬间四周空气激流涌动，六脉剑气大盛！\n"NOR,me,target);

	me->add("neili",-500);
	me->add("jingli",-300);
	me->add_temp("apply/pfm_attack",120+lev/10);
	me->add_temp("apply/pfm_damage",150+lev/5);
	me->add_temp("apply/damage",500);

	me->set_temp("yyz/6mai/6mai",1);
	call_out("check",1,me,target,lev);
	return 1;
}

int check(object me,object target, int lev)
{
	if (!me) return 1;
	if( !me->is_fighting()
	|| !living(me)
	|| me->is_ghost()
	|| (int)me->query("jiali")<50
	|| (int)me->query("neili")<500
	|| me->query("weapon")){
		message_vision(HIG"\n$N六脉真气运用完毕，气归丹田，神采奕奕！\n"NOR,me,target);
  	me->add_temp("apply/pfm_attack",-120-lev/10);
	  me->add_temp("apply/pfm_damage",-150-lev/5);
  	me->add_temp("apply/damage",-500);
		me->delete_temp("yyz/6mai/6mai");
		return 1;
	}
	call_out("check",1,me,target,lev);
     return 1;
}

int help(object me)
{
   write(WHT"\n一阳指之「"HIW"六脉神剑"WHT"」："NOR"\n\n");
   write(@HELP

     要求: 最大内力 4500 以上；
           当前内力 2000 以上；
           当前精力 1000 以上；
           六脉神剑等级 30 以上；
           特殊内功等级 250 以上；
           基本内功等级 250 以上；
           加力 50 以上；
           激发指法为一阳指。

HELP
   );
   return 1;
}
