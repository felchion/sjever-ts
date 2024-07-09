// By Darken@SJ
// Modified by Numa@Sj 99-11-12 2:46
// Modified by Darken@SJ

inherit F_SSERVER;
#include <ansi.h>
#include <combat_msg.h>

string perform_name() { return HIG"落英劈空掌"NOR; }

int query_ap() {return 3;}

int perform(object me, object target)
{
	int lvl;


	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
		return notify_fail("劈空掌只能对战斗中的对手使用。\n");

	if (me->query_temp("weapon"))
		return notify_fail("你拿着武器怎么能使用劈空掌！\n");

	if ((int)me->query_skill("bihai-chaosheng", 1) < 200 )
		return notify_fail("你的碧海潮生功修为未到，无法打出劈空掌。\n");
	if ((int)me->query_skill("luoying-zhang",1) < 200 )
		return notify_fail("你的掌法修为不足，使不出劈空掌。\n");
	if ((int)me->query("max_neili") < 5000)
		return notify_fail("你现在内力太弱，不能打出劈空掌。\n");
	if ((int)me->query("neili") < 2000)
		return notify_fail("你现在内力太弱，不能打出劈空掌。\n");

	if (me->query_skill_prepared("strike") != "luoying-zhang" || me->query_skill_mapped("strike") != "luoying-zhang")
		return notify_fail("你准备的技能不对！\n");
	
	if (me->query_skill_mapped("force") != "bihai-chaosheng")
		return notify_fail("你激发的内功不对！\n");
	lvl = me->query_skill("bihai-chaosheng", 1);

	me->receive_damage("neili", 500+lvl);
  me->add_temp("apply/damage",lvl*2);
  me->add_temp("apply/pfm_damage",230+lvl/4);
  me->add_temp("apply/pfm_attack",240+lvl/10);
	me->set_temp("thd/pikong",1);
	me->set_temp("apply/action/pfm",HIW"$N一声清啸扬起一掌遥遥劈向$n，落英神剑掌风挟带著澎湃汹涌的碧海潮生功席卷而至，其势殊不可挡");
	COMBAT_D->do_attack(me, target,0,random(2)?0:3);
  me->add_temp("apply/damage",-lvl*2);
  me->add_temp("apply/pfm_damage",-230-lvl/4);
  me->add_temp("apply/pfm_attack",-240-lvl/10);
	me->delete_temp("thd/pikong",1);
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
        write(HIG"\n落英神剑掌之「劈空掌」："NOR"\n");
        write(@HELP

        劈空掌凌厉绝伦，是桃花岛武学中少有的刚猛招式。

        要求：  碧海潮生功等级 200 以上；
                落英神剑掌等级 200 以上；
                最大内力 5000 以上；
                当前内力 2000 以上；
HELP
        );
        return 1;
}

