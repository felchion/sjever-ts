inherit F_SSERVER;
 
#include <ansi.h>
#include <combat.h> 

int query_ap() {return 4;}
string perform_name(){ return HIW"迦楼裂天舞"NOR; }

void remove_effect(object me,object target,int lv);

int perform(object me, object target)
{
	int lv, i;

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) )
	return notify_fail("「迦楼裂天舞」只能在战斗中使用。\n");

	if( me->query_temp("weapon"))
	return notify_fail("你拿着武器怎么能使用「迦楼裂天舞」！\n");

	if( (int)me->query_skill("tianwang-zhua", 1) < 150 )
	return notify_fail("你的金翅天王爪还不够娴熟，使不出「迦楼裂天舞」绝技。\n");

	if (me->query_skill_prepared("claw") != "tianwang-zhua"
	|| me->query_skill_mapped("claw") != "tianwang-zhua")
	return notify_fail("你现在无法使用「迦楼裂天舞」进行攻击。\n");

	if( (int)me->query_skill("longxiang-boruo", 1) < 150 )
	return notify_fail("你的龙象般若功等级还不够，使不出「迦楼裂天舞」绝技。\n");

	if( me->query("max_neili") < 2000 )
	return notify_fail("你现在内力太弱，使不出「迦楼裂天舞」。\n");

	if( (int)me->query("neili") < 1000 )
	return notify_fail("你现在真气太弱，使不出「迦楼裂天舞」。\n");

	lv = me->query_skill("tianwang-zhua",1);

	me->add_temp("apply/pfm_attack", 150+lv/10);
	me->add_temp("apply/damage", 350+lv/2);
	me->add_temp("apply/pfm_damage", 140+lv/5);
	me->add_temp("apply/action/color", HIW);


	message_vision(HIC"$N两臂张开忽如一只大鸟腾空而起，只见$N双手微动，空中顿时幻出漫天爪影！", me, target);

	for( i=0; i < 4; i++ ){
		if (target && me->is_fighting(target) && target->query("qi")>0)
		COMBAT_D->do_attack(me, target);
	}

	me->add_temp("apply/pfm_attack", -150-lv/10);
	me->add_temp("apply/damage", -350-lv/2);
	me->add_temp("apply/pfm_damage", -140-lv/5);
	me->delete_temp("apply/action");

	me->start_busy(random(2));
	return 1;
}


int help(object me)
{
        write(RED"\n「迦楼裂天舞」："NOR"\n");
        write(@HELP
        

        要求：  最大内力 2000 以上；      
                当前内力 1000  以上；  
                金翅天王爪等级 150 以上；
                龙象般若功等级 150 以上；
                激发爪法为金翅天王爪；
                激发招架为金翅天王爪；
                且手无兵器；

                
HELP
        );
        return 1;
}


