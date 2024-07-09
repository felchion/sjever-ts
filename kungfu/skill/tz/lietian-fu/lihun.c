// lihun.c 离魂斩

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 4;}

string perform_name(){ return RED"离魂斩"NOR; }

int perform(object me, object target)
{
	int lvl;
	object weapon;
	weapon = me->query_temp("weapon");
	lvl = (int)me->query_skill("lietian-fu", 1);
	if( !target ) target = offensive_target(me);

	if( !target || !me->is_fighting(target) )
	return notify_fail("「离魂斩」只能在战斗中对对手使用。\n");

	if (!weapon || weapon->query("skill_type") != "axe"
	|| me->query_skill_mapped("axe") != "lietian-fu")
	return notify_fail("你使用的兵刃不正确，无法使用「离魂斩」！\n");

	if( (int)me->query_skill("lietian-fu",1) < 120 )
	return notify_fail("你的裂天斧法不够娴熟，不能使用「离魂斩」！\n");

	if( (int)me->query_skill("axe",1) < 100 )
	return notify_fail("你的基本斧法等级不够，不能使用「离魂斩」！\n");

	if( (int)me->query_skill("guiyuan-tunafa", 1) < 100 )
	return notify_fail("你的归元吐纳法等级不够，不能使用「离魂斩」！\n");

	if( (int)me->query("max_neili") < 1000 )
	return notify_fail("你的内力太弱，不能使用「离魂斩」！\n");

	if( (int)me->query("neili") < 500 )
	return notify_fail("你的内力太少了，无法使用出「离魂斩」！\n");

	me->add("neili", - 500);
	
	message_vision(RED"\n$N使出「离魂斩」，每一式中都带着奇异的韵律，似乎时间都被这韵律所凝结，猛然间斧影如闪电似惊虹，如海浪般卷来。\n"NOR, me,target);
	me->add_temp("apply/pfm_damage", 180+lvl/5);
	me->add_temp("apply/damage", 500+lvl/2);
	me->add_temp("apply/pfm_attack", 240+lvl/10);
	me->set_temp("apply/action/color", HIY);
 
  me->add_temp("apply/pfm_hurt",180+lvl/2);	
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3,2);
	me->add_temp("apply/pfm_hurt",-180-lvl/2);
	me->add_temp("apply/pfm_attack", -60);
	
	if (present(target,environment(me)) && me->is_fighting(target))		
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
  
	if (present(target,environment(me))	&& me->is_fighting(target))
  	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);

	me->add_temp("apply/pfm_damage", -180-lvl/5);
	me->add_temp("apply/damage", -500-lvl/2);
	me->add_temp("apply/pfm_attack", -180-lvl/10);
	me->delete_temp("apply/action/color");
	me->start_busy(1);
	return 1;
}

int help(object me)
{
        write(HIY"\n裂天斧法之「离魂斩」："NOR"\n");
        write(@HELP
        
        要求：最大内力 1000 以上；
              当前内力  500 以上；
              裂天斧法等级 120 以上；                
              基本斧法等级 100 以上；
              裂天斧法为斧法；
              裂天斧法为招架；                
HELP
        );
        return 1;
}