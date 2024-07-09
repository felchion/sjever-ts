// by darken@SJ

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 2;}

string perform_name(){ return HBBLU"旋风快掌"NOR; }
int perform(object me, object target)
{
	int lvl;
	lvl = (int)me->query_skill("tiezhang-zhangfa", 1);
	if( !target ) target = offensive_target(me);

	if( !target || !me->is_fighting(target) )
		return notify_fail("「旋风快掌」只能在战斗中使用。\n");
	if( me->query_temp("weapon"))
		return notify_fail("你拿着武器怎么能使用「旋风快掌」！\n");
	if( lvl < 100 )
		return notify_fail("你的铁掌掌法不够娴熟，使不出「旋风快掌」。\n");
	if(me->query_skill("guiyuan-tunafa",1) < 100 )
		return notify_fail("你的归元吐呐法不够娴熟，使不出「旋风快掌」。\n");
	if(me->query_skill_mapped("strike") != "tiezhang-zhangfa" )
		return notify_fail("你现在无法使用「旋风快掌」！\n");
	if( (int)me->query("max_neili") < 1000)
		return notify_fail("你现在内力太弱，使不出「旋风快掌」。\n");
	if( (int)me->query("neili") < 500 )
		return notify_fail("你现在真气太弱，使不出「旋风快掌」。\n");

	message_vision(HBBLU"\n$N一提气，一声长啸，双掌猛往$n左右双肩拍去！\n"NOR,me,target);
	
	me->add("neili", -50);
	me->add_temp("apply/pfm_force",160+lvl/5);
	me->add_temp("apply/force",350+lvl/2);
	me->add_temp("apply/pfm_attack",120+lvl/10);
	
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(3)?0:3);
	if(objectp(target) && me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(3)?0:3);
	if(objectp(target) && me->is_fighting(target) && lvl > 300)
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?0:3);
	
	me->add_temp("apply/pfm_force",-160-lvl/5);
	me->add_temp("apply/force",-350-lvl/2);
	me->add_temp("apply/pfm_attack",-120-lvl/10);
	
	me->add_busy(random(2));
	return 1;
}

int help(object me)
{
   write(HBBLU"铁掌掌法之「旋风快掌」："NOR"\n");
   write(@HELP

     要求: 最大内力 2000 以上；
           当前内力大于 500 以上；
           铁掌掌法等级 100 以上；
           归元吐呐法等级 100 以上；
           激发掌法为铁掌掌法。
HELP
   );
   return 1;
}