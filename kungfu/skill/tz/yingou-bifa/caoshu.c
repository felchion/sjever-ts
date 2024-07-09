//caoshu.c 草书
#include <ansi.h>
#include <combat.h>


inherit F_SSERVER;

int query_ap() {return 6;}

string perform_name(){ return HIM"草书"NOR; }
int perform(object me, object target)
{
	string msg;
	int lvl;
	object weapon = me->query_temp("weapon");

	if (!weapon || weapon->query("skill_type") != "brush")
	return notify_fail("你手里没有笔，无法写出草书！\n");

	if( !target ) target = offensive_target(me);

	if( !target || !me->is_fighting(target) )
	return notify_fail("草书只能在战斗中对对手使用。\n");

	if( (lvl =(int)me->query_skill("yingou-bifa",1)) < 120 )
	return notify_fail("你的银钩笔法还不够娴熟，不会写草书！\n");

	if( (int)me->query_skill("literate",1) < 120 )
	return notify_fail("你的读书写字等级不够，不能写出草书！\n");

	if( (int)me->query_skill("guiyuan-tunafa",1) < 140 )
	return notify_fail("你的归元吐纳法等级不够，不能写出草书！\n");

	if( (int)me->query("max_neili") < 2000 )
	return notify_fail("你的内力太弱，写不出龙飞凤舞的草书! \n");

	if( (int)me->query("neili") < 1000 )
	return notify_fail("你的内力太少了，写出的草书无此意念！\n");

	if (me->query_skill_mapped("brush") != "yingou-bifa"
	|| me->query_skill_mapped("parry") != "yingou-bifa")
	return notify_fail("你现在无法写出「草书」！\n");

	if (!objectp(weapon = me->query_temp("weapon")) || weapon->query("skill_type") != "brush")
	return notify_fail("你手中无笔，如何写字。\n");
	

	msg = HIM"$N长袖飞舞，狂奔疾走，出招全然不依章法。只见其如疯如癫、如酒醉、如中邪，\n"+
	"笔意淋漓，笔走龙蛇！原来$N这时所书，正是唐代张旭的「自言帖」。张旭号称\n"+
	"「草圣」，乃草书之圣。\n" NOR;
	message_vision(msg,me,target);	
	
	me->add_temp("apply/pfm_damage", 160+lvl/5);
	me->add_temp("apply/damage", 500+lvl/2);
	me->add_temp("apply/pfm_attack", 180+lvl/10);
	me->set_temp("apply/action/color", HIM); 

	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
	
	if (present(target,environment(me)) && me->is_fighting(target))		
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
  
	if (present(target,environment(me))	&& me->is_fighting(target))
  	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
  	
  if (present(target,environment(me))	&& me->is_fighting(target))
  	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);	
  
  if (present(target,environment(me))	&& me->is_fighting(target))
  	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);	
	

	me->add_temp("apply/pfm_damage", -160-lvl/5);
	me->add_temp("apply/damage", -500-lvl/2);
	me->add_temp("apply/pfm_attack", -180-lvl/10);
	me->delete_temp("apply/action/color");
	me->start_busy(1);
	return 1;
}


int help(object me)
{
        write(HIM"\n银钩笔法之「草书」："NOR"\n");
        write(@HELP
        
        要求：最大内力 2000 以上；
              当前内力 1000 以上；
              归元吐纳法等级 120 以上；                
              银钩笔法等级 120 以上；
              读书写字等级 120 以上；
              激发银钩笔法为笔法；
               
HELP
        );
        return 1;
}