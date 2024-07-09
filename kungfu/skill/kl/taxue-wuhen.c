// taxue-wuhen.c踏雪无痕 
inherit SKILL;

string *dodge_msg = ({
"$n一式「雁落平沙」，变招奇速，右足向前踢出，身子已然腾起，轻轻巧巧地落在一旁。\n",
"$n一式「玉龙飞起」，纵身高跃，半空中一折一转，已飘身在数丈之外。\n",
"$n一式「夏日消融」，两足足跟不动，足尖左磨，身子随之右转，卸开了$N这一招。\n",
"$n一式「枯松倒挂」，只见$n身子后仰，双掌往地下一撑，身子借势倒射而出，$N便已打$n不着。\n",
"$n一式「晴雪飞滩」，纵高伏低，东奔西闪，身形飘忽不定，$N手无足措，只得回招自守。\n",
"$n一式「冰丝带雨」，突然反手一招，竟然是两败俱伤的打法，趁$N手忙脚乱安全脱身。\n",
"$n一式「寒江钓雪」，不招不架，只微微一侧身，$N犹如暴风骤雨般的进攻便即落空。\n",
"$n一式「鱼龙潜跃」，陡然间身形拔起，直冲云霄，直如鹰隼，几个盘旋，飘飘荡荡落在数丈之外。\n",
"$n一式「踏雪无痕」，身形急速旋转，几下就划到数丈之外。\n",
});

int valid_enable(string usage) { return usage == "dodge"; }

int valid_learn(object me)
{
	if ( me->query_skill("xuantian-wuji", 1) < 10)
		return notify_fail("你的玄天无极功等级太低了。\n");
	return 1; 
}

string query_dodge_msg()
{
	return dodge_msg[random(sizeof(dodge_msg))];
}

int practice_skill(object me)
{
	if( me->query("jingli") < 30 )
		return notify_fail("你的体力太差了，不能练踏雪无痕。\n");
	me->receive_damage("jingli", 25);
	return 1;
}

string perform_action_file(string action)
{       
	return __DIR__"kl/taxue-wuhen/" + action;
}
