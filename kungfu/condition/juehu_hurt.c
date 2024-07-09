#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{  
	if( !living(me))
		message("vision", me->name() + "浑身一阵哆嗦，冷得牙齿「咯咯」响。\n", environment(me), me);
	else {
		tell_object(me, HIB "你忽然感到体内精元流泄，头昏沉沉的。\n" NOR );
		message("vision", HIB+ me->name(1) + "突然两腿发软，两手抱成一团，冷得直发抖。\n"NOR, environment(me), me);
	}
	me->receive_wound("jing", me->query("max_jing")/20 , "精元枯竭而");
	me->add_busy(2);
	me->apply_condition("juehu_hurt", duration - 1);
	if( duration < 2 ) return 0;
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "hurt";
}
