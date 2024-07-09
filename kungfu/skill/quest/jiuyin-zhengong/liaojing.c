#include <ansi.h>

int exert(object me)
{
	if( me->is_fighting() )
		return notify_fail("战斗中运功疗伤？找死吗？\n");
	if ((int)me->query_skill("jiuyin-zhengong", 1) < 180)
		return notify_fail("你的九阴真功修为还不够。\n");
	if( (int)me->query("neili") < 500 )
		return notify_fail("你的真气不够。\n");
	if( (int)me->query("eff_jing") >= (int)me->query("max_jing"))
		return notify_fail("你的精神状态良好。\n");

	message_vision(HIW"$N深深的吸了口气，九阴真气运转一周天，精神看来好多了。\n" NOR,me);

        me->receive_curing("jing", (int)me->query_skill("force")*2/3);
	me->add("neili", -100);
	me->start_exert(1);
	me->start_busy(2);
	return 1;
}

string exert_name(){ return HIW"疗精诀"NOR; }

int help(object me)
{
        write(HIW"\n九阴真功之「疗精诀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 500 以上；
                九阴真功等级 180 以上。

HELP
        );
        return 1;
}
