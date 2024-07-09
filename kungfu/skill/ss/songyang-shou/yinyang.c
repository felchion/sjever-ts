// Created by Numa 2000.1.25
// Modified by darken@SJ

#include <ansi.h>
#include <combat.h> 
inherit F_SSERVER;

string perform_name() {return HIY"大阴阳手"NOR;}
int query_ap() {return 3;}

int perform(object me, object target)
{
	int lvl;

	if( !target ) target = offensive_target(me);

	if( !target
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target))
	return notify_fail("大阴阳手绝技只能对战斗中的对手使用。\n");

	if( lvl = (me->query_skill("songyang-shou", 1) < 140) )
	return notify_fail("你的嵩阳手火候不够。\n");

	if( (int)me->query_skill("hand", 1) < 140 )
	return notify_fail("你的基本招架火候不够。\n");

	if( (int)me->query("max_neili") < 2500 )
	return notify_fail("你的内力修为不够。\n");

	if( (int)me->query("neili") < 800 )
	return notify_fail("你的真气不够。\n");

	if( (int)me->query("jingli") < 500 )
	return notify_fail("你的精力不够。\n");

	if (!me->query("ss/yy"))
	return notify_fail("你不会大阴阳手绝技。\n");

	if (me->query_skill_mapped("force") != "hanbing-zhenqi")
	return notify_fail("你用什么为基础来使大阴阳手绝技?\n");

	if (me->query_skill_prepared("hand") != "songyang-shou"
	|| me->query_skill_mapped("hand") != "songyang-shou")
	return notify_fail("你现在无法使用嵩阳手。\n");

	if (me->query_temp("weapon"))
	return notify_fail("大阴阳手绝技只能空手使用！\n");

	if( me->query_temp("ss_yinyang"))
	return notify_fail("你正在使用大阴阳手绝技。\n");

	me->add("neili", -300-lvl/2);
	message_vision(HIY "\n$N突然向后倒窜，双掌向着$n发力遥击！\n" NOR,me, target);

  me->set_temp("ss_yinyang",1);
	me->set_temp("apply/action/pfm",HIB "\n$N双掌一阴一阳，阳掌先出，阴掌却后发而先至，一股寒气直逼$n而去");
  me->set_temp("apply/action/hit", BLU"$n闪避不及，被这阴寒掌力打在身上，霎时间透体剧寒，感觉冻僵了一般。\n\n"NOR);       

	me->add_temp("apply/force",300+lvl/2);
	me->add_temp("apply/pfm_force",150+lvl/5);
	me->add_temp("apply/pfm_attack",150+lvl/10);

  
  if (COMBAT_D->do_attack(me, target,0,random(2)?0:3))	
		 target->add_condition("cold_poison", 2+lvl/250);
		
  if(me->is_fighting(target)) {
		me->set_temp("apply/action/pfm",HIR"$n行动一缓，转眼$N的阳掌又至，只觉一股炙热的掌风席卷而来");
	  me->set_temp("apply/action/hit", RED"$n意欲闪避，已是不能，身体立似火烧火燎般痛楚！\n"); 	 
	  if (COMBAT_D->do_attack(me, target,0,random(2)?0:3))
	  	target->add_condition("hot_poison", 2+lvl/250);
	}
  
  me->delete_temp("ss_yinyang");
  me->delete_temp("apply/action/");
  me->add_temp("apply/force",-300-lvl/2);
	me->add_temp("apply/pfm_force",-150-lvl/5);
	me->add_temp("apply/pfm_attack",-150-lvl/10);

	me->start_busy(1);

	return 1;
}

int help(object me)
{
        write(HIW"\n嵩阳手之「大阴阳手」："NOR"\n\n");
        write(@HELP
        要求：  当前内力  800 以上；
                最大内力 2500 以上；
                嵩阳手等级 140 以上；
                寒冰真气等级 140 以上；
                基本手法等级 120 以上；
                激发手法为嵩阳手；
                激发内功为寒冰真气；

HELP
        );
        return 1;
}