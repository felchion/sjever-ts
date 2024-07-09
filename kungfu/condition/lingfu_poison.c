#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me,int duration)
{
	if(!environment(me)) return CND_CONTINUE;
	me->receive_wound("qi",50);
	me->receive_wound("jing",50);
	me->set("neili",0);
	me->add_busy(2);
	me->set_temp("last_damage_from","醉仙灵芙毒发作");
	if( !living(me) ) message("vision",HIY+ me->name() + HIY"痛苦地哼了一声。\n"NOR, environment(me), me);
	else 
		message_vision(HIY"$N感到微微有点头晕。\n"NOR,me);
	if(duration < 1) return 0;
	me->apply_condition("lingfu_poison",duration - 1);
	return CND_CONTINUE;
}

string query_type(object me)
{
        return "poison";
}
	