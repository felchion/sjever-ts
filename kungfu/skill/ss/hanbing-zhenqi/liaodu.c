// liaodu.c, 疗毒
// Lklv add help at 2001.10.06

#include <ansi.h>
inherit F_SSERVER;

string exert_name(){ return HIB"疗毒诀"NOR; }

int exert(object me,object target)
{
	string msg;
	if( me->is_fighting())
		return notify_fail("战斗中运功？找死吗？\n");

	if ( me->query_skill("hanbing-zhenqi", 1) < 120)
		return notify_fail("你的寒冰真气修为还不够。\n");

	if ( me->query("neili") < 1000 )
		return notify_fail("你的真气不够。\n");

	if ( !target) target = me;

	if ( target != me ) {
		if ( ! target->query_condition("cold_poison"))
			return notify_fail("你只能为"+target->name(1)+"治疗寒冰毒。\n");
		if ( target->is_busy() || target->is_fighting())
			return notify_fail(target->name(1)+"正忙着呢。\n");
		msg = HIB"$N坐在地上，深深的吸了一口气，开始为$n运功疗毒。\n" NOR;
		message_vision(msg, me, target);
		target->clear_condition("cold_poison");
		target->add_busy(2+random(3));
	}
	else {
		if(!me->query_condition("cold_poison"))
			return notify_fail("你只能治疗寒冰毒。\n");
		msg = HIB"$N坐在地上，深深的吸了一口气，开始运功疗毒。\n"NOR;
		message_vision(msg, me);
		me->clear_condition("cold_poison");
	}
	me->add("neili", -500);
	me->start_busy(2 + random(3));
	return 1;
}

int help(object me)
{
	write(HIB"\n寒冰真气之「疗毒诀」："NOR"\n\n");
        write(@HELP
	治疗寒冰毒。
	要求：  寒冰真气  120  级以上；
		当前内力  1000 以上。
                当前气超过最大气的 1/3 以上。

HELP
        );
        return 1;
}
