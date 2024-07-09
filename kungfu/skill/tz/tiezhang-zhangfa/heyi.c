// 铁掌掌法 -- 阴阳合一
// Modified by snowman@SJ 19/12/2000
// By Spiderii 加入450 lv后必出tianlei
#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

int query_ap() {return 5;}

string perform_name(){ return HBBLU"阴阳合一"NOR; }
int perform(object me, object target)
{
	int lvl;

	if( !target ) target = offensive_target(me);

	if( !target
	|| !me->is_fighting(target)
	|| !living(target)
	|| environment(target)!= environment(me))
	  return notify_fail("「阴阳合一」只能在战斗中对对手使用。\n");

	if( me->query_temp("weapon")) {
		if( me->query_temp("tzzf") ){
			if ( (int)me->query_skill("tiezhang-zhangfa",1) < 220 )
			return notify_fail("你必须空手才能使用「阴阳合一」！\n");
		}
		else
		return notify_fail("你必须空手才能使用「阴阳合一」！\n");
	}

	if( (int)me->query_skill("tiezhang-zhangfa", 1) < 200 )
	  return notify_fail("你的铁掌掌法还不够娴熟，使不出「阴阳合一」绝技。\n");

	if( (int)me->query_skill("guiyuan-tunafa", 1) < 200 )
	  return notify_fail("你的归元吐纳法等级不够，使不出「阴阳合一」绝技。\n");

	if (me->query_skill_mapped("force") != "guiyuan-tunafa")
	  return notify_fail("你所使用的内功不对。\n");

	if( (int)me->query_skill("force") < 230 )
	  return notify_fail("你的内功等级不够，不能使用「阴阳合一」。\n");

	if (me->query_skill_prepared("strike") != "tiezhang-zhangfa"
	|| me->query_skill_mapped("strike") != "tiezhang-zhangfa"
	|| me->query_skill_mapped("parry") != "tiezhang-zhangfa")
	  return notify_fail("你现在无法使用「阴阳合一」！\n");

	if( me->query_skill_mapped("force") != "guiyuan-tunafa")
  	return notify_fail("你现在的内功用不了「阴阳合一」！\n");

	if( (int)me->query("max_neili") < 1500)
  	return notify_fail("你现在内力太弱，使不出「阴阳合一」。\n");

	if( (int)me->query("neili") < 1000 )
	  return notify_fail("你现在真气太弱，使不出「阴阳合一」。\n");


	lvl = (int)me->query_skill("tiezhang-zhangfa", 1);
	message_vision(HIW"\n$N退了两步，突然身子微侧，紧跟着身形猛进直击$n！\n"NOR, me, target);

	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_hurt",400+lvl/2);
	me->add_temp("apply/pfm_attack",180+lvl/10);

	me->set_temp("apply/action/pfm", HIW"$N猛一吐气，单掌有如推门，另一掌却是迅疾无比的一推即收，双劲合一，去势如虎"NOR);

	COMBAT_D->do_attack(me, target,0,random(2)?0:3,2);

	if(objectp(target) && me->is_fighting(target)){
		me->set_temp("apply/action/pfm", HIR"紧接着$N身体一转，到了$n身旁，左掌顺力拍出，这一招看似平和，实是内涵罡气，霸道无比"NOR);
		COMBAT_D->do_attack(me, target, 0, random(2)?0:3,2);
	}
	me->add_temp("apply/pfm_hurt",-400-lvl/2);
	me->add_temp("apply/pfm_attack",-180-lvl/10);


	if( me->query_skill("tiezhang-zhangfa",1) > 300 ){
		tell_object(me, HIY"\n紧接着你掌势一变，开始凝聚秘传的“天雷气”……\n"NOR);
		me->start_call_out( (: call_other, __DIR__"tianlei", "tianlei_hit", me, 2 :), 1 );
	}
	return 1;
}

int help(object me)
{
        write(WHT"\n铁掌掌法「阴阳合一」："NOR"\n");
        write(@HELP
        这是裘千仞铁掌功的十三绝招之一，叫作「阴阳合一」，最是猛恶无比。
        而且据帮中护法说，还看过帮主在此招之后出过一式杀招！
        
        要求：  内力 1000 以上；      
                最大内力 1500 以上；
                精力 100 以上；  
                后天膂力 33 以上；
                铁掌掌法等级 140 以上；
                归元吐纳法等级 140 以上；    
                且手无兵器。
HELP
        );
        return 1;
}
