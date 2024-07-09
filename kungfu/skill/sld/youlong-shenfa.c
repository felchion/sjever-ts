// Skill: youlong-shengfa.c 游龙身法
// 19990811001

inherit SKILL;
#include <ansi.h>
string *dodge_msg = ({
"只见$n一招「飞龙在天」，身形忽地向上高高纵起，躲过了$N这一招。\n",
"$n身形向下一伏，一招「倦龙入巢」，「嗖」地一声从$N的胯下钻过。\n",
"$n突地身形向下一倒，使出一招「土龙翻身」，几个翻滚，向一旁避过。\n",
"但见$n一招「困龙归林」，身形向后轻轻一纵，避开了$N这一招。\n",
"$n一招「游龙千移」，身形连晃数下，硬是避过了$N这一招。\n",
"$n不慌不忙地使出一招「惊龙百惧」，往旁边一下子窜出数尺，正好让过了$N这一招。\n",
"只见$n身形一闪，使出一招「神龙一现」，忽地就转到了$N身后。\n",
});

int valid_enable(string usage) 
{
	return usage == "dodge"; 
}

int valid_learn(object me)
{
	if ( me->query_skill("dulong-dafa", 1) < 10)
		return notify_fail("你的毒龙大法等级太低了。\n");
	return 1; 
}

string query_dodge_msg(string limb)
{
	return dodge_msg[random(sizeof(dodge_msg))];
}

int practice_skill(object me)
{
	if( (int)me->query("jingli") < 30 )
		return notify_fail("你的精力太差了，不能练习游龙身法。\n");
	me->receive_damage("jingli", 25);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"sld/youlong-shenfa/" + action;
}
