// By River@SJ

#include <ansi.h>
void heal_done(object me, int count);

int exert(object me)
{
	int i;
	if (me->is_fighting())
		return notify_fail("战斗中运功疗伤？找死吗？\n");

	if ((int)me->query_skill("longxiang-boruo", 1) < 100)
		return notify_fail("你的内功修为还不够。\n");

	if ((int)me->query_skill("medicine", 1) < 80)
		return notify_fail("你的本草术理修为还不够。\n");

	if( (int)me->query("neili") < 1000 )
		return notify_fail("你的真气不够。\n");

	if( (int)me->query("eff_qi") >= (int)me->query("max_qi"))
		return notify_fail("没受伤疗什么伤？\n");

	if (me->query("eff_qi")+me->query_temp("apply/qi") < ((me->query("max_qi") + me->query_temp("apply/qi"))/4))
		return notify_fail("你已经受伤过重，经受不起真气震荡！\n");

	message_vision(HIW"$N收蹑心神，屏息静气，缓缓开始运功疗伤。\n"NOR,me);

	i = me->query("max_qi");
	i -= me->query("eff_qi");
	i /= 500;

	me->receive_curing("qi", 500);
	me->set("qi", me->query("eff_qi"));
	me->add("neili", -100);

	if ( i  > 0 ) {
		call_out("heal_done", 1, me, i);
		me->start_busy(i);
		return 1;
	}
	message_vision(HIW"$N长吸一口气，精神抖擞的站了起来。\n"NOR, me);
	me->start_exert(1, "聚血");
	me->start_busy(random(2));
	return 1;
}

void heal_done(object me, int count)
{
	if(!me) return;
	if( me->is_fighting() 
	|| count < 0
	|| me->query("neili") < 200) {
		message_vision(HIW"$N长吸一口气，精神抖擞的站了起来。\n"NOR, me);
		me->start_exert(1, "聚血");
		me->start_busy(-1);
		me->start_busy(random(2));
		return;
	}
	me->receive_curing("qi", 500);
	me->set("qi", me->query("eff_qi"));
	me->add("neili", -100 );
	call_out("heal_done", 1, me, count -1);
}

string exert_name(){ return HIW"聚血"NOR; }

int help(object me)
{
        write(HIW"\n龙象般若功之「聚血」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                本草术理等级 80 以上；
                龙象般若功等级 100 以上；
                受伤后的气上限不小于原气上限的 25%。

HELP
        );
        return 1;
}
