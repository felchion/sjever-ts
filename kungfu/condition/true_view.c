#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{

	me->apply_condition("true_view", duration - 1);

	if(!random(10))
		tell_room(environment(me), RED+me->name() + "看起来目光如炬，两眼烁烁有神。\n"+NOR, ({ me }));


	if( duration < 1 )
	{
		tell_object(me, HIW"你双目一阵酸痛，又恢复到原来的视野。\n"NOR);
		return 0;
	}
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "true_view";
}

string query_name(){return MAG"真实视野"NOR;}