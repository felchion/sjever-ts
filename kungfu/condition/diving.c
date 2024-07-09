#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{
	int l_temp; 

	me->set_temp("last_damage_from", "被水淹");
	if(living(me)) {
		tell_object(me, HIY "你无法呼吸，心跳加速，头脑「嗡嗡」响。\n" NOR );
		message("vision", me->name() + "艰难地呼出一口气，看样子快不行了。\n", environment(me), me);
	}
	l_temp=120-(int)me->query_skill("force", 1)/5;
	me->add("qi",-l_temp);
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "drown";
}
