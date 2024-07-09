// bahuang-gong.c 八荒六合唯我独尊功

inherit FORCE;

#include "../force.h"

int valid_enable(string usage) { return usage == "force"; }

int valid_learn(object me)
{	
	if (me->query_skill("force", 1) < 10)	
		return notify_fail("你的基本内功还需要进一步提高。\n");
	if ((int)me->query_skill("bahuang-gong", 1) > me->query_skill("force", 1) + 10	
	 && me->query_skill("bahuang-gong", 1) >= 200 )	
		return notify_fail("你的基本功火候未到，必须先打好基础才能继续提高。\n");
	return valid_public(me);
}

int practice_skill(object me)
{	
	return notify_fail("八荒六合唯我独尊功只能用学(learn)的来增加熟练度。\n");
}

string exert_function_file(string func)
{	
	return __DIR__"other/bahuang-gong/" + func;
}
