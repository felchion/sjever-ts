// hunyuan-yiqi.c 少林 混元一气功
// dts@xkx, cleansword@xkx

#include <ansi.h>
inherit FORCE;

int valid_enable(string usage)
{
	return usage == "force";
}

#include "../force.h"

int valid_learn(object me)
{
	int nb, nf, nh, ns;
	nb = (int)me->query_skill("buddhism", 1);
	nf = (int)me->query_skill("force", 1);
	nh = (int)me->query_skill("hunyuan-yiqi", 1);
	ns = (int)me->query("guilty");

	if ( me->query("gender") != "男性" )
		return notify_fail("你非童男之体，不能练习混元一气功。\n");

	if ( nb < 100 && nb <= nh )
		return notify_fail("你的禅宗心法修为不够，无法领会更高深的混元一气功。\n");

	if ( nf < 15)
		return notify_fail("你的基本内功火候还不够，无法领会混元一气功。\n");

	if ( ns > 0 ) 
		return notify_fail("你屡犯僧家数戒，无法领会更高深的混元一气功。\n");

	return valid_public(me);
}


int practice_skill(object me)
{
	return notify_fail("混元一气功只能用学(learn)的来增加熟练度。\n");
}

string exert_function_file(string func)
{
	return __DIR__"hunyuan-yiqi/" + func;
}
mapping exercise_msg(object me)
{
  return ([
    "status_msg" : HIR + me->name()+"脸上红光隐现，气象庄严。" NOR,
      "start_my_msg" : "你五心向天，成五心向天的姿势，排除一切杂念，内息顺经脉缓缓流动。\n",
          "start_other_msg" : me->name()+"五心向天，一会工夫，从口鼻处缓缓吐出白气，白气越吐越浓，渐渐笼罩了全身。\n",
    "halt_msg" : "$N长出一口气，将内息急速退了回去，站了起来。\n",
              "end_my_msg" : "你将内息走了个小周天，流回丹田，收功站了起来。\n",
                  "end_other_msg" : "只见笼罩"+me->name()+"的白气渐渐消散，又从口鼻处吸了回去。\n"
                    ]);
}
