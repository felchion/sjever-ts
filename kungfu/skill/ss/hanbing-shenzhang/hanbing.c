// snowman
// Modified by Numa 2000.1.25

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 2;}

string perform_name() {return BLU"寒冰神掌"NOR;}

int perform(object me, object target)
{

	int lvl;

	if( !target ) target = offensive_target(me);

	if( !target
	||  !target->is_character()
	||  !me->is_fighting(target)
	||  !living(target))
	return notify_fail("你不在战斗中。\n");

	if( (lvl = (int)me->query_skill("hanbing-shenzhang", 1)) < 150 )
	return notify_fail("你的寒冰神掌修为不够。\n");

	if( (int)me->query_skill("force", 1) < 150 )
	return notify_fail("你的基本内功修为不够。\n");

	if ( me->query_skill_mapped("force") != "hanbing-zhenqi")
	return notify_fail("你的内功不对！\n");

	if (me->query_skill_prepared("strike") != "hanbing-shenzhang"
	|| me->query_skill_mapped("strike") != "hanbing-shenzhang")
	return notify_fail("你没有运用寒冰神掌！\n");

	if( me->query("max_neili") < 2000 )
	return notify_fail("你的内力修为不够！\n");

	if( me->query("neili") < 800 )
	return notify_fail("你的真气不够！\n");


	me->set_temp("apply/action/pfm", BLU "\n$N将已修练了十余年的“寒冰真气”注于食指之上，拚着大耗内力，手指一伸，飞点$n的胸口"NOR);
	me->set_temp("apply/action/hit", BLU "$N一指点在$n胸口，待其运内力抵抗，将寒毒顺他“天池穴”直逼过去$n体内。\n"NOR);

	me->receive_damage("neili", 300+lvl);
	me->add_temp("apply/pfm_hurt",500+lvl/2);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	
	if(COMBAT_D->do_attack(me, target,0,random(2)?0:3,2)) 
	  target->add_condition("cold_poison", 3+lvl/200);;
	
	me->add_temp("apply/pfm_hurt",-500-lvl/2);
	me->add_temp("apply/pfm_attack",-180-lvl/10);
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/");

	me->add_busy(1);

	return 1;
}

int help(object me)
{
        write(BLU"\n「寒冰神掌」："NOR"\n");
        write(@HELP        

        要求：  最大内力 2000 以上；
                当前内力 800 以上；
                寒冰神掌等级 150 以上；
                寒冰真气等级 150 以上；
                激发寒冰神掌为掌法；
                
HELP
        );
        return 1;
}