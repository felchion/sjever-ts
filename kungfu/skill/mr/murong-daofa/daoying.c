//    /kungfu/skill/xingyi-zhang/daoying.c pfm daoying
// by leontt /22/6/2000

#include <ansi.h>
inherit F_SSERVER;
#include <skill_class.h>

int query_ap() {return 4;}

string perform_name(){ return HIR"刀影掌"NOR; }

int perform(object me,object target)
{
	int lv = me->query_skill("murong-daofa", 1);
	object weapon = me->query_temp("weapon");
	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) )
	return notify_fail("刀影掌只能对战斗中的对手使用。\n");

	if( (int)me->query_skill("shenyuan-gong", 1) < 100 )
	return notify_fail("你的内功还未练成，不能使用刀影掌！\n");

	if( (int)me->query_skill("murong-daofa", 1) < 100 )
	return notify_fail("你的刀法还未练成，不能使用刀影掌！\n");

	if (!weapon
	|| weapon->query("skill_type") != "blade"
	|| me->query_skill_mapped("blade") != "murong-daofa")
	return notify_fail("你手里没有刀，无法使用刀影掌！\n");

	if((int)me->query_skill("xingyi-zhang", 1) < 100 )
	return notify_fail("你的掌法还未练成，不能使用刀影掌！\n");

	if(me->query_skill_mapped("force") != "shenyuan-gong")
	return notify_fail("你的内功不是神元功，无法使用刀影掌！\n");

	if(me->query_skill_mapped("strike") != "xingyi-zhang"
	|| me->query_skill_prepared("strike") != "xingyi-zhang")
	return notify_fail("你没有用掌，无法使用刀影掌！\n");

	if((int)me->query("max_neili") < 1200 )
	return notify_fail("你现在内力修为不够，不能使用刀影掌！\n");

	if((int)me->query("neili") < 600 )
	return notify_fail("你现在内力不足，不能使用刀影掌！\n");

	message_vision(HIR"\n$N大吼一声使出慕容绝技「刀影掌」，只见$P掌随刀走，片片刀光中夹杂着阵阵掌风一起奔向$n！\n"NOR, me,target);
	me->add("neili", -200);
	me->add("jingli", -50);

	me->add_temp("apply/pfm_attack", 150+lv/10);
	me->add_temp("apply/damage", 500+lv/2);
	me->add_temp("apply/pfm_damage", 250+lv/4);
  me->set_temp("apply/action/class",ZHENG);
  me->set_temp("apply/action/color",HIG);
  me->set_temp("apply/action/skill","xingyi-zhang");
  
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"));

	if(objectp(target) && me->is_fighting(target)){
		weapon->unequip();
		COMBAT_D->do_attack(me, target);
		weapon->wield();
	}

	if(objectp(target) && me->is_fighting(target))
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?0:3);

	if(objectp(target) && me->is_fighting(target)){
		weapon->unequip();
		COMBAT_D->do_attack(me, target, 0, random(2)?0:3);
		weapon->wield();
	}

	if(objectp(target) && me->is_fighting(target) && random(lv) > 150)
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"),3);

	me->add_temp("apply/pfm_attack", -150-lv/10);
	me->add_temp("apply/damage", -500-lv/2);
	me->add_temp("apply/pfm_damage", -250-lv/4);
  me->delete_temp("apply/action/");
  
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
        write(HIR"\n「刀影掌」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 600 以上；
                最大内力 1200 以上；
                慕容刀法等级 100 以上；
                神元功等级 100 以上；
                星移掌等级 100 以上；
                激发掌法为星移掌；
                备掌法为星移掌；
                激发刀法为慕容刀法；
                且手中持刀。
HELP
        );
        return 1;
}