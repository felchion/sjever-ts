// qianzhu.c 千蛛万毒
// by Lsxk@HSBBS

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 4;}

string perform_name(){ return HIW"万"HIB"毒"NOR;}

int perform(object me, object target)
{
	string msg;
	int lvl,i,count;

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !target->is_character()
	|| !me->is_fighting(target) || !living(target) )
	return notify_fail("「千蛛万毒」绝招只能对战斗中的对手使用。\n");

	if( (int)me->query_skill("qianzhu-wandushou", 1) < 150 )
	return notify_fail("你的千蛛万毒手功力还远远不够，无法使用「千蛛万毒」绝招！\n");

	if( me->query_temp("weapon") )
	return notify_fail("你必须空手才能使用「千蛛万毒」！\n");

	if (me->query_skill_prepared("hand") != "qianzhu-wandushou" ||
	me->query_skill_mapped("parry") != "qianzhu-wandushou")
	return notify_fail("你必须先将千蛛万毒手运用于手法之中才行。\n");

	if( (int)me->query("max_neili") < 2000 )
	return notify_fail("你的内力修为不足，使不出「千蛛万毒」绝招！\n");

	if( (int)me->query("neili") < 500 )
	return notify_fail("你的内力不足，使不出「千蛛万毒」绝招！\n");

	if( (int)me->query_skill("hand", 1) < 150 )
	return notify_fail("你的手法不够娴熟，使不出「千蛛万毒」绝招！\n");

	i = me->query_skill("beauty",1);
	if (i > me->query("max_pot") - 100) 
	  i = me->query("max_pot") - 100;
	i /= 20;
	i += me->query("per") ;
	
	lvl = (int)me->query_skill("qianzhu-wandushou", 1);


	msg= HIR "\n$N暗运千蛛万毒功，将体内积蓄剧毒逼于双手处，喝道：“小心了！”，便是如急风暴雨般地攻向$n！\n"NOR;
	message_vision(msg, me, target);
	me->receive_damage("neili", 500+lvl);
	me->receive_damage("jingli", 500);

	me->add_temp("apply/pfm_damage", 130+lvl/5+i);
	me->add_temp("apply/damage", 200+lvl/2+i*5);
	me->add_temp("apply/pfm_attack", 150+lvl/10+i*5);
	me->set_temp("apply/action/color", HBBLU);

	for (count=0;count<4;count++)
	  COMBAT_D->do_attack(me, target);
	  
	me->add_temp("apply/pfm_damage", -130-lvl/5-i);
	me->add_temp("apply/damage", -200-lvl/2-i*5);
	me->add_temp("apply/pfm_attack", -150-lvl/10-i*5);
	me->delete_temp("apply/action/color");
	me->delete_temp("qzwd_pfm");

	return 1;
}



int help(object me)
{
   write(WHT"\n千蛛万毒手「"HIW"万"HIB"毒"WHT"」："NOR"\n");
   write(@HELP

   要求：  内力 500 以上；
           最大内力 2000 以上； 
           千蛛万毒手 150 以上；
           基本手法 150 以上；
           空手备手法为千蛛万毒手。

HELP
   );
   return 1;
}
