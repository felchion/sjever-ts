// Skill: chongling-jian.c 冲灵剑法
// Date: Look 99/03/25

inherit SKILL;

mapping *action = ({
([
	"action" : "$N一招「青梅如豆」，手中$w豆粒般刺向$n的$l",
	"lvl": 0,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N使出「柳叶似眉」，手中$w飘忽不定，刺向$n的$l",
	"lvl": 10,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N一招「雾中初见」，$w化成千百剑影，向$n劈砍下去",
	"lvl": 20,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N向前跨上一步，手中$w使出「雨后乍逢」直刺$n的$l",
	"lvl": 30,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N手中的$w一晃，迸出无数道劲气，使出「同生共死」直刺$n的周身大穴",
	"lvl": 40,
	"damage_type" : "刺伤"
]),
});

int valid_enable(string usage) { return (usage == "sword") || (usage == "parry"); }

int valid_learn(object me)
{
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "sword")
		return notify_fail("你使用的武器不对。\n");
	if ( me->query("max_neili") < 50)
		return notify_fail("你的内力不够。\n");
	if ( me->query_skill("zixia-gong", 1) < 20)
		return notify_fail("你的华山内功火候太浅。\n");
	return 1;
}


mapping query_action(object me, object weapon)
{
	int i, level, j;
	level = me->query_skill("chongling-jian",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
				"force": 100 +random(250),
				"dodge": random(20)-10,
				"parry": random(10),
				"damage": 40 +random(60),
			]);
		}
	}
}

int practice_skill(object me)
{
	if ((int)me->query("jingli") < 30 || me->query("neili") < 10 )
		return notify_fail("你的体力不够练冲灵剑法。\n");
	me->receive_damage("jingli", 20);
	me->add("neili", -5);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"hs/chongling-jian/" + action;
}
