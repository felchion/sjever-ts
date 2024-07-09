// yinyun-ziqi.c 氤氲紫气

#include <ansi.h>
inherit FORCE;

int valid_enable(string usage) { return usage == "force"; }

#include "../force.h"

int valid_learn(object me)
{
	int lvl = (int)me->query_skill("yinyun-ziqi", 1);
	int t = 1, j;

	for( j = 1; j < lvl / 10; j++) t *= 2;
	if( t > 5000 ) t = 5000;
	if( me->query("gender") == "无性" && lvl > 49 )
		return notify_fail("你无根无性，阴阳不调，难以领会高深的氤氲紫气神功。\n");

	if( me->query("class") == "bonze" )
		return notify_fail("氤氲紫气讲究阴阳调合，有违佛家六根清净之意，" +RANK_D->query_respect(me)+"欲修此功，已是罪过。\n");

	if( (int)me->query_skill("force", 1) < 15 )
		return notify_fail("你的基本内功火候还不够。\n");

	if( (int)me->query_skill("yinyun-ziqi", 1) > me->query_skill("force", 1) + 10
	&& me->query_skill("yinyun-ziqi", 1) >= 200 )
		return notify_fail("你的基本功火候未到，必须先打好基础才能继续提高。\n");

	if( (lvl > 10 && lvl < 100 && (int)me->query("shen") < t * 200)
	|| ( lvl >=100 && (int)me->query("shen") < 10000) )
		return notify_fail("你的侠义正气太低了。\n");

	if( me->query("menggu") )
		return notify_fail("你已投降蒙古，无法再进一步提高氤氲紫气。\n");

	return valid_public(me);
}

int practice_skill(object me)
{
	if( (int)me->query_skill("yinyun-ziqi", 1) >= 200 ) {
		if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200) {
			me->improve_skill("yinyun-ziqi", (int)me->query_int());
			me->add("neili", -100); 
			me->add("potential", -1*(1+random(3)));
			return 1;
		}
		else return notify_fail("你现在的修为不足以提高氤氲紫气了。\n");
	}
	else return notify_fail("你现在的氤氲紫气修为只能用学(learn)的来增加熟练度。\n");
}

string exert_function_file(string func)
{
	return __DIR__"wd/yinyun-ziqi/" + func;
}

mapping exercise_msg(object me)
{
	return ([
		"status_msg" : MAG + me->name()+"头顶氤气蒸蒸，神色威严" NOR,
		"start_my_msg" : "你盘膝而坐，双目紧闭，深深吸一口气引入丹田，慢慢让一股内息在周身大穴流动，渐入忘我之境。\n",
		"start_other_msg" : me->name()+"盘膝坐了下来，双手虚握成拳，不一会，头顶冒出氤氤紫气。\n",
		"halt_msg" : "$N微一簇眉，将内息压回丹田，长出一口气，站了起来。\n",
		"end_my_msg" : "你将内息在体内运行十二周天，返回丹田，只觉得全身暖洋洋的。\n",
		"end_other_msg" : me->name()+"长出一口气，一抖衣襟，站了起来，眼中精光一闪。\n"
	]);
}
