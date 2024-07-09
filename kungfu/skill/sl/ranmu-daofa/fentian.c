// fentian.c 举火焚天
// by hunthu

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 3;}
string perform_name() {return HIR"举火焚天"NOR;}

int perform(object me, int amount)
{
	string msg;
	object weapon, target;
	int lvl;
	weapon = me->query_temp("weapon");
	lvl= me->query_skill("ranmu-daofa",1);

	if( !target ) target = offensive_target(me);

	if( !target
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("「举火焚天」只能对战斗中的对手使用。\n");

	if (!weapon || weapon->query("skill_type") != "blade"
	|| me->query_skill_mapped("blade") != "ranmu-daofa")
	return notify_fail("你现在无法使出「举火焚天」之技！\n");

	if (weapon->query("unique"))
	return notify_fail("你手中之刀过于锋利，影响你运功发火！\n");

	if( lvl < 120 )
	return notify_fail("你的燃木刀法火候不够，无法「举火焚天」。\n");

	if((int)me->query_skill("blade", 1) < 120 )
	return notify_fail("你的基本刀法还不够娴熟，使不出「举火焚天」绝技。\n");

	if( (int)me->query_skill("yijin-jing", 1) < 120 )
	return notify_fail("你的易筋经修为不够，使不出「举火焚天」绝技。\n");

	if ((int)me->query_skill("strike",1) < 120)
	return notify_fail("你的掌法修为太差，无法使出「举火焚天」绝技。\n");

	if (me->query_skill_mapped("force") != "yijin-jing")
	return notify_fail("没有易筋经作为根基，是无法使出「举火焚天」绝技的！\n");

	if (me->query_temp("fentian"))
	return notify_fail("你正在使用「举火焚天」绝技！");

	if (me->query("max_neili") < 1500)
	return notify_fail("你的内力修为不够。\n");

	if (me->query("neili") < 1000)
	return notify_fail("你的内力不够。\n");

  lvl = me->query_skill("taiji-quan",1);
	
	msg =HIR"\n$N猛然一声大喝，手中的"+weapon->name()+HIR"如一道流星腾空而起，一阵筚拨之声，洒下漫天火焰，\n"+
	"刀头指处，一股烈焰熊熊而出。$N聚气于双掌，在间不容发之际，往空虚劈九九八十一掌";	

	me->set_temp("apply/action/pfm",msg);
  me->set_temp("apply/action/hit", HIR"$n见火焰扑面卷来，不免大惊失色，但躲闪无及，被熊熊大火围在当中。\n"NOR);       
	target->set_temp("apply/action/parry", HIR"$n虽早有防备，但乍见火光耀眼也不禁大吃一惊，急忙跃开规避，也已大是狼狈。\n"NOR);
	target->set_temp("apply/action/dodge", HIR"$n虽早有防备，但乍见火光耀眼也不禁大吃一惊，急忙跃开规避，也已大是狼狈。\n"NOR);	
	  
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_hurt",500+lvl/2);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	COMBAT_D->do_attack(me, target,weapon,random(2)?0:3,2);
	me->add_temp("apply/pfm_hurt",-500-lvl/2);
	me->add_temp("apply/pfm_attack",-180-lvl/10);	 
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
        write(HIR"\n燃木刀法之「举火焚天」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 1500 以上；
                燃木刀法等级 120 以上；
                易筋经等级 120 以上；
                基本刀法等级 120 以上；
                激发刀法为燃木刀法；

HELP
        );
        return 1;
}   

