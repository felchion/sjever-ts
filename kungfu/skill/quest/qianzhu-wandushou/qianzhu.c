// qianzhu.c 千蛛万毒
// by Lsxk@HSBBS

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 12;}

int perform_normal(object,object,int);

string perform_name(){ return HIR"千"HIG"蛛"NOR;}

int perform(object me, object target)
{
	string msg;

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


	msg= HIB "\n$N暗运千蛛万毒于双手，脸上蓝气时隐时现！\n"NOR;
	message_vision(msg, me, target);
	perform_normal(me,target,6);
	return 1;
}

int perform_normal(object me,object target,int count)
{
	int lvl,i=3,j,k;
	
	k = me->query_skill("beauty",1);
	if (k > me->query("max_pot") - 100) 
	  k = me->query("max_pot") - 100;
	k /= 20;
	k += me->query("per") ;
	
	tell_object(me,sprintf("K=%d\n",k));
	if( !target ) target = offensive_target(me);
	if (me->is_fighting()
	&& me->query_skill_prepared("hand") == "qianzhu-wandushou"
	&& me->query_skill_mapped("parry") == "qianzhu-wandushou"
	&& count >0)
	{
	  me->add_temp("apply/pfm_damage", 150+lvl/4+k*2);
	  me->add_temp("apply/damage", 300+lvl/2+k*5);
	  me->add_temp("apply/pfm_attack", 150+lvl/10+k*5);
		me->set_temp("apply/action/color",HIB);
		for(j=0;j<i;j++)
		  COMBAT_D->do_attack(me, target);
		call_out("perform_normal",2,me,target,count--);
	  me->add_temp("apply/pfm_damage", -150-lvl/4-k*2);
	  me->add_temp("apply/damage", -300-lvl/2-k*5);
	  me->add_temp("apply/pfm_attack", -150-lvl/10-k*5);
	  me->delete_temp("apply/action/color");
		return 1;
	}
	else
	  message_vision(HIB"\n$N的千蛛万毒绝技使用完毕，长长地呼出了一口气！\n"NOR,me);
	return 1;

}

int help(object me)
{
   write(WHT"\n千蛛万毒手「"HIR"千"HIG"蛛"WHT"」："NOR"\n");
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
