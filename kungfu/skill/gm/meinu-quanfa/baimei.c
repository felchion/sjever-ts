// baimei.c 百媚横生

#include <ansi.h>

inherit F_SSERVER;
int query_ap() {return 3;}

string perform_name(){ return HIM"百媚横生"NOR; }

int perform(object me, object target)
{
	int i,lvl;
	
	if(!target) target = offensive_target(me);

	if(!target)
	  return notify_fail("你没在战斗中呢。\n");
	
	if(!me->is_fighting(target))
		return notify_fail("你没在战斗中呢。\n");

	if(me->query("gender") != "女性")
		return notify_fail("只有女性才能使用「百媚横生」！\n");
		
	if( (lvl=(int)me->query_skill("meinu-quanfa", 1)) < 120 )
		return notify_fail("你的美女拳法还不够纯熟，无法施展「百媚横生」。\n");

	if(me->query_skill_prepared("cuff") != "meinu-quanfa")
		return notify_fail("你现在无法施展「百媚横生」。\n");
			
	if(me->query_skill_mapped("parry") != "meinu-quanfa")
		return notify_fail("你没有使用美女拳法作为招架！\n");
	
	if(me->query_skill_mapped("cuff") != "meinu-quanfa")
		return notify_fail("你没有使用美女拳法作为招架！\n");
	
	if ( me->query_temp("weapon"))
		return notify_fail("你手持武器，如何使用「百媚横生」绝技？\n");
	
	if(me->query("max_neili")<1200)
		return notify_fail("你内力不够，无法使用「百媚横生」绝技。\n");
	
	if(me->query("max_neili")<800)
		return notify_fail("你内力不够，无法使用「百媚横生」绝技。\n");
				
	message_vision(HIW"\n$N嫣然一笑，形容可谓百媚千娇惊艳之至，手上招式更是有若行云流水！\n"NOR,me);
	
	i = me->query_skill("beauty",1);
	if (i > me->query("max_pot") - 100) 
	  i = me->query("max_pot") - 100;
	i /= 20;
	i += me->query("per") ;
	
	me->add("neili", -300-lvl/2);
	me->add_temp("baimei", 1);
	me->add_temp("apply/pfm_force", 40+i*3+lvl/5);
	me->add_temp("apply/force", 130+i*5+lvl/5);
	me->add_temp("apply/pfm_attack", 80+i*2+lvl/10);
	me->add_temp("apply/action/color",HIM);

  if (present(target,environment(me)) && me->is_fighting(target))
	  COMBAT_D->do_attack(me, target, 0, random(80)>i?1:3);
	if (present(target,environment(me)) && me->is_fighting(target))
		COMBAT_D->do_attack(me, target, 0, random(70)>i?1:3);
	if (present(target,environment(me)) && me->is_fighting(target))
		COMBAT_D->do_attack(me, target, 0, random(60)>i?1:3);  
  
	me->add_temp("apply/pfm_force", -40-i*3-lvl/5);
	me->add_temp("apply/force", -130-i*5-lvl/5);
	me->add_temp("apply/pfm_attack", -80-i*2-lvl/10);
	me->delete_temp("apply/action/color");  
  me->delete_temp("baimei");
  
	me->start_busy(1+random(2)); 
	return 1;
}


int help(object me)
{
        write(HIM"\n美女拳法之「百媚横生」："NOR"\n\n");
        write(@HELP
        要求：  当前内力  800 以上；
                最大内力 1200 以上；
                美女拳法等级 120 以上；
                激发拳法为美女拳法；
                女性专用;
        
        注：容貌越高威力越大。        

HELP
        );
        return 1;
}   