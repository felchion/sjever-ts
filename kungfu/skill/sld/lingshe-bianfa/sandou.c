 // sandou 灵蛇三抖

#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return HIY"灵蛇三抖"NOR;}
int query_ap() {return 3;}

int perform(object me, object target)
{
	int lvl;
	object weapon;

	weapon = me->query_temp("weapon");
	lvl = me->query_skill("lingshe-bianfa",1);

	if( !target ) target = offensive_target(me);

	if( !target
	|| !me->is_fighting(target) )
	return notify_fail("「灵蛇三抖」只能在战斗中对对手使用。\n");

	if(!weapon
	|| weapon->query("skill_type") != "whip"
	|| me->query_skill_mapped("whip") != "lingshe-bianfa")
	return notify_fail("你手里没有鞭，无法使用「灵蛇三抖」！\n");

	if((int)me->query_skill("lingshe-bianfa",1) < 120 )
	return notify_fail("你的灵蛇鞭法不够娴熟，不能使用「灵蛇三抖」！\n");


	if((int)me->query_skill("dulong-dafa", 1) < 120 )
	return notify_fail("你的毒龙大法等级不够，不能使用「灵蛇三抖」！\n");

	if((int)me->query("neili") < 1000 )
	return notify_fail("你的内力太少，不能使用「灵蛇三抖」！\n");

	if(me->query_skill_mapped("force") != "dulong-dafa")
	return notify_fail("你现在使用的内功与毒龙大法相抵触，不能使用「灵蛇三抖」。\n");

	message_vision(HIY"\n$N连续抖动手中鞭子，"+weapon->query("name")+HIY+"循着诡异的轨迹向$n袭去！\n"NOR, me,target);
	
	me->add("neili", -500-lvl/2);
	
	me->add_temp("apply/pfm_damage", 200+lvl/5);
	me->add_temp("apply/damage", 300+lvl);
	me->add_temp("apply/pfm_attack", 120+lvl/5);
	me->set_temp("apply/action/color", HIW);

	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);

	if (present(target,environment(me)) && me->is_fighting(target)) {
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
	}

	if (present(target,environment(me))	&& me->is_fighting(target))
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);

	me->add_temp("apply/pfm_damage", -200-lvl/5);
	me->add_temp("apply/damage", -300-lvl);
	me->add_temp("apply/pfm_attack", -120-lvl/5);
	me->delete_temp("apply/action/color");
	me->start_busy(1);
	me->delete_temp("sandou");
	return 1;
}

int help(object me)
{
        write(HIY"\n灵蛇鞭法之「灵蛇三抖」："NOR"\n");
        write(@HELP
        
        要求：当前内力 1000 以上；
              灵蛇鞭法等级 120 以上；                
              毒龙大法等级 120 以上；
              灵蛇鞭法为鞭法；
              手中持鞭；                
HELP
        );
        return 1;
}