// duoming.c 苗家剑法之「连环夺命诀」。
// Write By lsxk@hsbbs 5/10/2007

#include <ansi.h>

inherit F_SSERVER;


int query_ap() {return 3;}

string perform_name() {return HIY"连环夺命诀"NOR;}
int perform(object me,object target)
{
	object weapon;
	int lvl,count;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)|| !me->is_fighting(target) || !living(target)
	|| environment(target)!=environment(me))
	return notify_fail("此「连环夺命诀」只能对战斗中的对手使用。\n");

	if( (lvl = (int)me->query_skill("miaojia-jianfa", 1)) < 250 )
	return notify_fail("你的剑法还未练成，不能使用「连环夺命诀」！\n");

	if(!objectp(weapon = me->query_temp("weapon"))
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "miaojia-jianfa")
	return notify_fail("你手里没有剑，无法使用「连环夺命诀」！\n");

	if((int)me->query("max_neili") < 3000 )
	return notify_fail("你的内力修为不够，不能使用「连环夺命诀」！\n");
	if((int)me->query("neili") < 800 )
	return notify_fail("你现在真气不足，不能使用「连环夺命诀」！\n");
	if((int)me->query("jingli") < 500 )
	return notify_fail("你现在精力不足，不能使用「连环夺命诀」！\n");
	if((int)me->query_skill("sword", 1) < 250 )
	return notify_fail("你的基本剑法不够娴熟，无法使用「连环夺命诀」。\n");

	message_vision(HIY"\n$N"HIY"使出苗家剑法之「"HIW"连环夺命诀"HIY"」，但见$P手中"+weapon->name()+HIY"一阵疾舞，招招欲夺$n性命"HIY"！\n"NOR, me,target);

	me->receive_damage("neili", 500+lvl);
	me->receive_damage("jingli", 500);

	me->add_temp("apply/pfm_damage", 130+lvl/5);
	me->add_temp("apply/damage", 200+lvl/2);
	me->add_temp("apply/pfm_attack", 150+lvl/10);
	me->set_temp("apply/action/color", HIC);

	for (count=0;count<4;count++)
	  COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);
	  
	me->add_temp("apply/pfm_damage", -130-lvl/5);
	me->add_temp("apply/damage", -200-lvl/2);
	me->add_temp("apply/pfm_attack", -150-lvl/10);
	me->delete_temp("apply/action");

	me->start_busy(random(2));
	return 1;
}


int help(object me)
{
   write(WHT"\n苗家剑法「"HIW"连环夺命诀"WHT"」："NOR"\n");
   write(@HELP
   此招「连环夺命诀」乃苗家剑法之精髓，使用后，能在战斗中
   接二连三的向对手施以攻击，往往另对手防不胜防！另若，使
   用者会武林绝学冷泉神功之「冷泉」绝技，则相辅相成，效果
   骇然！

   要求：  内力 800 以上；
           精力 500 以上； 
           最大内力 3000 以上； 
           苗家剑法 250 以上；
           基本剑法 250 以上；

HELP
   );
   return 1;
}
