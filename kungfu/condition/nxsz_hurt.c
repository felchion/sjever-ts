#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{
	int i;

	tell_object(me, HIR"忽然一丝热流自体內升起，缓缓的浸入你四肢百骸。原来你中的凝血神爪内伤发作了！\n" NOR );
	message("vision", HIR + me->name() + "忽然感觉一丝热流自体內升起，缓缓的浸入四肢百骸。\n"NOR, environment(me), me);

	i = me->query_skill("force");
	i = i/2 + random(i/2);
	me->add_busy(2+random(3));
	me->receive_damage("qi", i, "凝血神爪内伤发作");
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "hurt";
}
