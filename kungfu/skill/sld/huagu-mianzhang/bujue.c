// bujue.c 绵绵不绝
// Modified by lqz@TS 06/02/2011

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

string perform_name() {return HIC"绵绵不绝"NOR;}

//void attackob(object me,object target,int lvl,int hits,int);
//void dohit(object,object);

int perform(object me, object target)
{
	string msg;
	int lvl,i,j;

	lvl=(int)me->query_skill("huagu-mianzhang", 1);


	if( !target ) target = offensive_target(me);

	if( !target
	|| !target->is_character()
	|| !me->is_fighting(target) )
	    return notify_fail("「绵绵不绝」只能在战斗时用！\n");

	
	if ( me->query_skill_prepared("strike") != "huagu-mianzhang"
	|| me->query_skill_mapped("strike") != "huagu-mianzhang"
	|| me->query_skill_mapped("parry") != "huagu-mianzhang" )
	    return notify_fail("你没有激发化骨绵掌，不能使用「绵绵不绝」！\n");


	if( objectp(me->query_temp("weapon")) )
	    return notify_fail("你必须空手才能用「绵绵不绝」！\n");


	if(lvl < 100 )
	    return notify_fail("你的化骨绵掌还不够熟练，不能使用！\n");


	if(me->query_skill("dulong-dafa",1) < 100 )
	    return notify_fail("你的毒龙大法功力不足，不能用绵绵不绝。\n");


	if ( me->query_skill_mapped("force") != "dulong-dafa" )
	    return notify_fail("你没有激发毒龙大法，不能使用「绵绵不绝」！\n");


	if((int)me->query("neili") < 800)
	    return notify_fail("你现在内力不足，不能用「绵绵不绝」！\n");


	if((int)me->query("jingli") < 300)
	    return notify_fail("你现在精力不足，不能用「绵绵不绝」！\n");


	if((int)me->query("jiali") < 50)
	    return notify_fail("你加力太少了，不能用「绵绵不绝」！\n");


	msg = HIY"\n$N忽然猛一吸气，双掌一立，向$n轻轻地拍出。\n"+
				HIC"似乎出手不重，但其波浪状的掌气后浪击前浪，一浪高过一浪，连绵不尽！\n"NOR;
	message_vision(msg, me, target);


	me->add_temp("apply/force", 300+lvl);
	me->add_temp("apply/pfm_force", 180+lvl/5);
	me->add_temp("apply/pfm_attack", 150+lvl/10);

  COMBAT_D->do_attack(me, target);
  if (me->is_fighting(target))
    COMBAT_D->do_attack(me, target, 0, random(2)?0:3);

	i = (lvl-100)/150;
  if (i>3) i = 3;
  for(j = 0; j < i; j++){   
    if (me->is_fighting(target)){
    	me->add_temp("apply/pfm_force", j*20);
      COMBAT_D->do_attack(me, target, 0, random(2)?0:3);
      me->add_temp("apply/pfm_force", -j*20);  
    }
  }
	
	me->add_temp("apply/force", -300-lvl);	
	me->add_temp("apply/pfm_force", -180-lvl/5);
	me->add_temp("apply/pfm_attack", -150-lvl/10);


	me->add("neili", -200-i*100);
	me->add("jingli", -100-i*50);
	me->start_busy(random(2));
	me->start_perform(2+random(2+i), "绵绵不绝");

	return 1;
}



int help(object me)
{
   write(WHT"\n化骨绵掌「"HIC"绵绵不绝"WHT"」："NOR"\n");
   write(@HELP

    要求：当前内力 800 以上；
          当前精力 300 以上；
          化骨绵掌等级 100 以上；
          毒龙大法等级 100 以上；

HELP
   );
   return 1;
}
