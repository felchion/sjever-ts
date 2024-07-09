// 随波逐流身法

inherit SKILL;

string *dodge_msg = ({
"$n一式「随波逐流」，身体便如大海中的一叶孤舟一样，随着$N的招式游走，浑不着力。\n",
"$n一式「风起云涌」，纵身高跃，半空中忽然倒退，轻飘飘地落在数丈之外。\n",
"$n一式「惊涛拍岸」，反倒抢向$N，于间不容发之际避开了$N这一招。\n",
"$n一式「顺水推舟」，身子便如游鱼般，轻轻一卸，就将$N袭来的招式推到一旁。\n",
"$n一式「大浪淘沙」，纵高伏低，趁$N眼花缭乱之时已避开了此招。\n",
"$n一式「击棹中流」，蹂身而上，竟是两败俱伤的打法，$N一楞之下，$n已安全脱身。\n",
"$n一式「星河倒悬」，双足微蹬，身体向后平平飞出，堪堪躲过了这招。\n",
"$n一式「天际归舟」，倏尔远游，只一霎间，身子竟已在十丈开外！\n",
});

int valid_enable(string usage)
{
	return usage == "dodge";
}

int valid_learn(object me)
{
	if ( me->query_skill("bihai-chaosheng", 1) < 10)
		return notify_fail("你的碧海潮生功等级太低了。\n");
	return 1; 
}

string query_dodge_msg(object me)
{
	return dodge_msg[random(sizeof(dodge_msg))];
}

int practice_skill(object me)
{
	if( (int)me->query("jingli") < 30 )
		return notify_fail("你的精力太差了，不能练随波逐流身法。\n");
	me->receive_damage("jingli", 25);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"thd/suibo-zhuliu/" + action;
}
