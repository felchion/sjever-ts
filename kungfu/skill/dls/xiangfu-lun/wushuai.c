// wushuai.c 天人五衰
// Rewrite By lsxk@hsbbs 2007/5/16

#include <ansi.h>

inherit F_SSERVER;

string perform_name(){ return HIY"天人五衰"NOR; }
int query_ap() {return 3;}
int perform(object me,object target)
{
	object weapon;
	int i,j,lv;

	if( !target ) target = offensive_target(me);

	if( !objectp (target) || !me->is_fighting(target) || !living(target)
	|| environment(target)!=environment(me))
	  return notify_fail("「天人五衰」只能对战斗中的对手使用。\n");

	if(me->query_skill_mapped("force") != "longxiang-boruo")
	  return notify_fail("你的内功不是龙象般若功，无法使用「天人五衰」！\n");

	if( (int)me->query_skill("longxiang-boruo", 1) < 150 )
	  return notify_fail("你的龙象般若功等级不够，无法使用「天人五衰」！\n");

	if( (int)me->query_skill("xiangfu-lun", 1) < 160 )
  	return notify_fail("你的金刚降伏轮还未练成，无法使用「天人五衰」！\n");

	if((int)me->query_skill("hammer", 1) < 160 )
	  return notify_fail("你的基本锤法不够娴熟，不能使用「天人五衰」！\n");

	if (!objectp(weapon = me->query_temp("weapon")) || weapon->query("skill_type") != "hammer"
	|| me->query_skill_mapped("hammer") != "xiangfu-lun")
	  return notify_fail("你必须使用锤法，才能使出「天人五衰」!\n");

	if(me->query_skill_mapped("parry") != "xiangfu-lun")
  	return notify_fail("「天人五衰」需要使用降伏轮法招架一切敌方攻击。\n");

	if((int)me->query("max_neili") < 3000 )
	  return notify_fail("你内力修为尚浅，使不出「天人五衰」。\n");

	if( (int)me->query("neili") < 1300 )
  	return notify_fail("你现在真气太弱，使不出「天人五衰」。\n");

	message_vision(HIY"\n只听“呜-呜-”之声大作，$N将手中法轮旋转掷出，使出「天人五衰」绝技，奋力掷出！\n"NOR, me,target);

	lv = (int)me->query_skill("xiangfu-lun",1);

	if( me->query_temp("xs/longxiang") ) {
		message_vision(MAG"$N更把龙象内劲附于兵刃之上，"+weapon->query("name")+MAG"通体流淌着异样的紫色光芒！\n\n"NOR, me,target);
		lv = lv*6/5;
	}

	me->add("neili", -250-lv);
	
	me->set_temp("xfl/wushuai", 1);
  me->add_temp("apply/pfm_attack", 150+lv/10);
  me->add_temp("apply/damage", 200+lv/2);
  me->add_temp("apply/pfm_damage", 100+lv/5);
	me->add_temp("apply/action/color",HIY);
	
	for( i=0; i < 5; i++ ){
		for(j=0;j<4;j++) {
		  if(!target || target->query("qi") < 0) target = offensive_target(me);
		  else break;
	  }
		if (target && me->is_fighting(target) && target->query("qi")>0)
		COMBAT_D->do_attack(me, target,me->query_temp("weapon"), random(3)?1:3);
	}

  me->add_temp("apply/pfm_attack", -150-lv/10);
  me->add_temp("apply/damage", -200-lv/2);
  me->add_temp("apply/pfm_damage", -100-lv/5);
  me->delete_temp("apply/action/color");  

	me->delete_temp("xfl/wushuai");
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
        write(HIY"\n金刚降伏轮之「天人五衰」："NOR"\n");
        write(@HELP
        
        要求：  龙象般若功等级 150 以上；
                金刚降伏轮法等级 160 以上；
                基本锤法等级 160 以上；
                最大内力 3000 以上；
                当前内力 1300 以上；
                手握法轮。
HELP
        );
        return 1;
}