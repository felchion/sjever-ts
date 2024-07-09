// by darken@SJ

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 3;}

string perform_name(){ return HBBLU"刀中掌"NOR; }


int perform(object me, object target)
{
	object weapon;
	int lv;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	||  !me->is_fighting(target)
	|| !living(target))
	return notify_fail("刀中掌只能对战斗中的对手使用。\n");

	if( (lv =(int)me->query_skill("liuye-daofa", 1) )< 100 )
	return notify_fail("你的柳叶刀法功力太浅，无法使用刀中掌攻击。\n");

	if( (int)me->query_skill("tiezhang-zhangfa", 1) < 100 )
	return notify_fail("你的铁掌掌法功力太浅，无法使用刀中掌攻击。\n");

	if( (int)me->query("max_neili") < 1500 )
	return notify_fail("你的内力太浅，无法使用刀中掌攻击。\n");

	if( (int)me->query("neili") < 1000 )
	return notify_fail("你的真气不足，无法使用刀中掌攻击。\n");

	if (!objectp(weapon = me->query_temp("weapon")) || weapon->query("skill_type") != "blade"
	|| me->query_skill_mapped("blade") != "liuye-daofa" ||
	me->query_skill_prepared("strike") != "tiezhang-zhangfa")
	return notify_fail("你的准备功夫不对。\n");

	me->add("neili", -200);
	me->add("jingli", -40);
	message_vision(HBBLU"$N呼呼呼连环三招，刀风中夹着两招铁掌掌法，迅速向$n砍去！\n" NOR,me,target);
	me->add_temp("apply/pfm_attack", 150+lv/10);
	me->add_temp("apply/damage", 500+lv/2);
	me->add_temp("apply/pfm_damage", 200+lv/4);
  me->set_temp("apply/action/color",BLU);
  me->set_temp("apply/action/skill","tiezhang-zhangfa");
  
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"),1);

	if(objectp(target) && me->is_fighting(target)){
		weapon->unequip();
		COMBAT_D->do_attack(me, target);
		weapon->wield();
	}

	if(objectp(target) && me->is_fighting(target))
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?0:3);


	me->add_temp("apply/pfm_attack", -150-lv/10);
	me->add_temp("apply/damage", -500-lv/2);
	me->add_temp("apply/pfm_damage", -200-lv/4);
  me->delete_temp("apply/action/");
  
	me->start_busy(random(2));
	return 1;
}


int help(object me)
{
   write(HBBLU"柳叶刀法之「刀中掌」："NOR"\n");
   write(@HELP

     要求: 最大内力 1500 以上；
           当前内力大于 1000 以上；
           柳叶刀法等级 100 以上；
           铁掌掌法等级 100 以上；
           归元吐呐法等级 100 以上；
           激发掌法为铁掌掌法。
HELP
   );
   return 1;
}
