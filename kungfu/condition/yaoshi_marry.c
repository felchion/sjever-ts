// yaoshi_marry.c

#include <ansi.h>
#include <login.h>

int update_condition(object me, int duration)
{
	if (duration < 1) {
		return 0;
	}
	if (!duration) return 0;
	me->apply_condition("yaoshi_marry", duration - 1);
	return 1;
}

string query_type(object me)
{
	return "marry";
}
