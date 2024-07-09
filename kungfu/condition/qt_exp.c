#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{

	me->apply_condition("qt_exp", duration - 1);

	if(random(10)==1)
	{
		tell_room(environment(me), me->name() + "看起来心智懵懂。\n", ({ me }));
		tell_object(me, CYN"在洛迦之心的作用下，你不知世事专致武学。\n"NOR);
	}

	if( duration < 1 )
	{
		tell_object(me, HIR"你觉得有些心智清明，又恢复了正常的思维。\n"NOR);
		return 0;
	}
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "qt_exp";
}

string query_name()
{
	return CYN"痴愚"NOR;
}