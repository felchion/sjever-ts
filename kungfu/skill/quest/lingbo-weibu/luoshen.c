#include <ansi.h>
#include <skill.h>
#include <combat.h>

inherit F_SSERVER;

int perform(object me)
{
	string msg;
	int i;

	if( (int)me->query_skill("lingbo-weibu",1) < 150 )
		return notify_fail("你的凌波微步还不够火候，使不出洛神。\n");
	if( (int)me->query("max_neili",1) < 3000 )
		return notify_fail("你的内力修为还不够火候，使不出洛神。\n");
	if( (int)me->query("neili", 1) < 1000 )
		return notify_fail("你现在内力不够。\n");
	if (me->query_temp("lbwb/ls"))
		return notify_fail("你正在使用洛神。\n");

	msg = HIC "$N越走越快，按合四相八卦之势,内息随着步法不住运转,转眼间犹如雾一样！\n\n"NOR;

	me->add("neili", -(300+random(300)));

	message_vision(msg, me);
	i = me->query_skill("lingbo-weibu", 1)/10 + me->query_int();
	i = i*2 - random(i/2);
	if (me->query_skill("lingbo-weibu", 1) < 450)
	i = i/2;
	me->add_temp("apply/pfm_dodge", 240);
	me->add_temp("apply/pfm_parry", 180);
	me->set_temp("lbwb/ls", i);
	me->start_perform(2, "洛神");
	call_out("remove_effect", 1, me, i, i);
	return 1;
}

void remove_effect(object me, int count)
{
	if (!me)
		return;
	if( me->query_skill_mapped("dodge") != "lingbo-weibu"
	|| me->query("neili") < 100
	|| count < 1)
	{
		me->add_temp("apply/pfm_dodge", -240);
		me->add_temp("apply/pfm_parry", -180);
		me->delete_temp("lbwb/ls");
		message_vision(HIW"$N内力不济，身法慢了下来。\n"NOR, me);
		return;
	}
	call_out("remove_effect", 1 , me, count -1);
}

int help(object me)
{
	write(HIW"\n凌波微步「洛神」："NOR"\n\n");
	write(@HELP
	
	要求：
		最大内力 3000 以上；
		当前内力 1000 以上；
		凌波微步等级 300 以上；
		激发轻功为凌波微步；
HELP
	);
	return 1;
}