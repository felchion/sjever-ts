// shendao.c  血刀经 [神刀]
// changed by emnil@sj  2/5/2000

#include <ansi.h>
#include <skill.h>
#include <combat.h>
#include <weapon.h>
inherit F_SSERVER;
 
string perform_name(){ return HIR"神刀穿心"NOR; }

int query_ap() {return 4;}

int perform(object me, object target)
{
	object weapon, ob;
	string msg, weaname;
	int lvl;

	if( !me->is_fighting() )
	return notify_fail("「神刀穿心」只能在战斗中使用。\n");

	if (!objectp(weapon = me->query_temp("weapon")) || (string)weapon->query("skill_type") != "blade")
	return notify_fail("你使用的武器不对。\n");

	if((int)me->query_skill("xuedao-jing",1) < 100)
	return notify_fail("你的刀法修为不够， 目前不能使用「神刀穿心」! \n");

	if((int)me->query_skill("longxiang-boruo",1) < 100)
	return notify_fail("你的龙象般若功修为不够， 目前不能使用「神刀穿心」! \n");

	if( me->query_skill_mapped("blade") != "xuedao-jing")
	return notify_fail("你必须使用血刀经，才能使用「神刀穿心」的绝技！\n");

	if( me->query_skill_mapped("force") != "longxiang-boruo")
	return notify_fail("你现在所使用的内功同「龙象般若功」向抵触，不能使出「神刀穿心」的绝技！\n");

	if( (int)me->query("max_neili") < 1000 )
	return notify_fail("你的内力修为不够！\n");
	if((int)me->query("neili")<500)
	return notify_fail("你的真气不够！\n");
	if((int)me->query("jingli")<200)
	return notify_fail("你的精力不够！\n");

	ob = me->select_opponent();

	if( !userp(me) && userp(ob) )
	return notify_fail("「神刀穿心」只能由玩家使用！\n");

	lvl = (int)me->query_skill("xuedao-jing",1);
	weapon = me->query_temp("weapon");
	weaname = weapon->query("name");
	msg = HIR "\n$N狂啸声中，运起「神刀穿心」秘诀，刹时身法如电，"NOR+weaname+HIR"一分为四，劈将出去！\n"NOR;
	message_vision(msg, me);

	me->add("neili", -500-random(300));
	
	if(me->query_temp("xs/longxiang")) {
		me->add_temp("apply/pfm_force", 120+lvl/5);
		me->add_temp("apply/focre", 300+lvl/3);
		message_vision(MAG"$N更把龙象内劲附于兵刃之上，"+weaname+MAG"通体流淌着异样的紫色光芒！\n"NOR, me,target);
	}
	me->add_temp("apply/damage", 200+lvl/2);
	me->add_temp("apply/pfm_damage", 150+lvl/5);
	me->add_temp("apply/pfm_attack", 180+lvl/10);

	if (present(ob,environment(me)) && me->is_fighting(ob))
	COMBAT_D->do_attack(me, ob, me->query_temp("weapon"), random(3)?0:3);
	if (present(ob,environment(me)) && me->is_fighting(ob))
	COMBAT_D->do_attack(me, ob, me->query_temp("weapon"), random(3)?0:3);
	if(lvl >150)
	if (present(ob,environment(me)) && me->is_fighting(ob))
	COMBAT_D->do_attack(me, ob, me->query_temp("weapon"), random(2)?0:3);
	if(lvl >200)
	if (present(ob,environment(me)) && me->is_fighting(ob))
	COMBAT_D->do_attack(me, ob, me->query_temp("weapon"), random(2)?0:3);


	if(me->query("neili")<0) me->set("neili",0);
	me->add("jingli", -80);
	me->start_busy(random(2));
	me->add_temp("apply/damage", -200-lvl/2);
	me->add_temp("apply/pfm_damage", -150-lvl/5);
	me->add_temp("apply/pfm_attack", -180-lvl/10);
	if(me->query_temp("xs/longxiang")) {
		me->add_temp("apply/pfm_force", -120-lvl/5);
		me->add_temp("apply/focre", -300-lvl/3);
	}
	return 1;
}

int help(object me)
{
        write(HIR"\n「神刀穿心」："NOR"\n");
        write(@HELP

        要求：  最大内力 1200 以上；
                当前内力 500 以上；
                血刀经等级 100 以上；
                龙象般若功等级 100 以上；
                
HELP
        );
        return 1;
}