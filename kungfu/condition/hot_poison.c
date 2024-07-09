#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{
	if( !living(me) ) {
		message("vision", me->name() + "全身变黑，散发出一阵焦糊味。\n", environment(me), me);
	}
	else {
		tell_object(me, HIR "忽然一股火热之气袭来，烫得你手脚发颤！\n" NOR );
		message("vision", HIR + me->name()+"突然全身冒出阵阵红烟，嘴唇干裂，双手胡乱地挥舞着。\n" NOR,
				environment(me), me);
	}
	if(userp(me))
	  me->receive_wound("qi", me->query("max_qi")/10,"火毒发作");
	else 
	  me->receive_wound("qi", me->query("max_qi")/5,"火毒发作");
	me->add_busy(2);
	me->apply_condition("hot_poison", duration - 1);
	if( duration < 1 ) return 0;
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "poison";
}
