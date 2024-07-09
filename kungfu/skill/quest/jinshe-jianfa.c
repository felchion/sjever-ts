// jinshen-jianfa.c 金蛇剑法
// by snowman@SJ 2000


inherit SKILL;

mapping *action = ({
([
	"action" : "$N剑式平走，一招「双蛇钻腋」，手中$w闪电般刺向$n的$l",
	"lvl" : 0,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N猛提身行，在空中腰身斜倾，手中$w一式「金蛇望月」直刺$n的$l",
	"lvl" : 20,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N使出「金蛇摆尾」，右膝微弯，$w斜刺,诡异的划向$n的$l",
	"lvl" : 50,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N身体越游越急，一招「灵蛇盘腾」，右手$w带着一道寒光，横穿$n的$l",
	"lvl" : 60,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N右手微转，纵身腾空，一招「飞龙戏蛇」，手中的$w绵绵抖动，袭向$n的$l",
	"lvl" : 80,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N一招「蛇影无踪」，身子稍侧，长臂直伸，左掌虚晃，右手奇招突起，$w反刺$n的$l",
	"lvl" : 100,
	"damage_type" : "刺伤"
]),
});

int valid_enable(string usage) { return (usage == "sword") || (usage == "parry"); }

int valid_learn(object me)
{
	object weapon;
	
	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "sword")
		return notify_fail("你必须使用金蛇剑才能进一步练习你的金蛇剑法。\n");

	if ( me->query("max_neili") < 1000)
		return notify_fail("你的内力不够。\n");
	if ( me->query_skill("sword", 1) < 100)
		return notify_fail("你的基本功太浅。\n");
	if ( !me->query("jinshe/jianfa"))
		return notify_fail("你无法修习金蛇剑法。\n");
        if ( me->query_skill("jinshe-zhangfa", 1) < 100)
		return notify_fail("你的金蛇游身掌火候太浅。\n");
	return 1;
}

string query_skill_name(int level)
{
	int i = sizeof(action);
	while (i--)
		if(level > action[i]["lvl"])
			return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
	int i, level, j;
	level   = (int) me->query_skill("jinshe-jianfa",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action":	action[j]["action"],
				"lvl":		action[j]["lvl"],
				"damage_type":	action[j]["damage_type"],
				"damage":	80 + random(140),
				"force":	200 + random(350),
				"dodge":	random(30)-10,
				"parry":	random(20),
			]);
		}
	}
}

int practice_skill(object me)
{
        if ((int)me->query("jingli") < 50)
		return notify_fail("你的体力不够练金蛇剑法。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练金蛇剑法。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"quest/jinshe-jianfa/" + action;
}
