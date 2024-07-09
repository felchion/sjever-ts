// by darken@SJ

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 3;}

string perform_name(){ return HIW"风舞九天"NOR; }

int perform(object me, object target)
{
	int lvl;
	if( !target ) target = offensive_target(me);
	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) 
	|| !living(target))
		return notify_fail("风舞九天只能对战斗中的对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
		return notify_fail("你必须空手使用「风舞九天」！\n");	
	if( me->query_temp("thd/kuangfeng") )
		return notify_fail("你正在使用「风舞九天」！\n");
	if( (int)me->query_skill("xuanfeng-tui", 1) < 100 )
		return notify_fail("你的旋风扫叶腿不够娴熟，无法施展出风舞九天。\n");
	if ((int)me->query_skill("bihai-chaosheng", 1) < 100)
		return notify_fail("你的碧海潮生功火候不够，无法施展出风舞九天。\n");
	if ( (int)me->query("max_neili") < 2000)
		return notify_fail("你的内力不够，无法施展出风舞九天。\n");
	if ( (int)me->query("neili") < 1500)
		return notify_fail("你的真气不够，无法施展出风舞九天。\n");
	if (me->query_skill_prepared("leg") != "xuanfeng-tui"
	|| me->query_skill_mapped("leg") != "xuanfeng-tui")
		return notify_fail("你现在无法使用风舞九天进行攻击！\n");
	if (me->query_skill_mapped("force") != "bihai-chaosheng")
		return notify_fail("你的碧海潮生功修为不够，无法使用风舞九天进行攻击！\n");

	lvl = (me->query_skill("xuanfeng-tui",1) + me->query_skill("luoying-zhang",1) )/2;

	message_vision(HIY"$N跃起在半空，双足乱舞，带起无数劲风，一式「凤舞九天」迅捷无伦地卷向$n。\n",me,target);
	
	me->add("neili", -500-lvl);
	me->add_temp("apply/pfm_damage",180+lvl/5);
	me->add_temp("apply/damage",350+lvl/2);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	me->add_temp("apply/action/color",HIW);
	
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(3)?0:3);
	if(objectp(target) && me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(3)?0:3);
	if(objectp(target) && me->is_fighting(target) && lvl > 300)
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?0:3);
	
	me->add_temp("apply/pfm_damage",-180-lvl/5);
	me->add_temp("apply/damage",-350-lvl/2);
	me->add_temp("apply/pfm_attack",-180-lvl/10);
	me->delete_temp("apply/action");
	
	me->add_busy(random(2));
	return 1;
}

int help(object me)
{
        write(HIW"\n旋风扫叶腿之「风舞九天」："NOR"\n");
        write(@HELP

        要求：  碧海潮生功等级 100 以上；
                旋风扫叶腿等级 100 以上；
                最大内力 2000 以上；
                当前内力 1000 以上；

HELP
        );
        return 1;
}