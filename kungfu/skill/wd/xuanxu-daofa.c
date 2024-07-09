#include <skill_class.h>
inherit SKILL;
string query_class() {return "妙";}

mapping *action = ({
([
	"action" : "$N刀锋斜引，身体跃起丈许,一招「劈风展翅」，手中的$w向$n的$l横劈过去",
	"lvl" : 0,
	"skill_name" : "劈风展翅",
	"damage_type" : "割伤"
]),
([
	"action" : "$N一招「风拂山岗」，手中$w向$n的下盘划去，招未使老，忽又直挑$n的$l",
	"lvl" : 20,
	"skill_name" : "风拂山岗",
	"damage_type" : "割伤"
]),
([
	"action" : "$N斜跨两步，抢到$n左侧，一招「抽刀断水」,$w自左向右平平横扫$n的前胸",
	"lvl" : 40,
	"skill_name" : "抽刀断水",
	"damage_type" : "割伤",
]),
([
	"action" : "$N一招「拨云见日」，手中的$w向$n虚砍一刀，刀刃忽又转向$n的$l砍去",
	"lvl" : 60,
	"skill_name" : "拨云见日",
	"damage_type" : "割伤"
]),
([
	"action" : "$N身随刀进，青光闪跃，一招「苍鹰望月」，直攻$n右肩，半徒刀锋兜了个半圆，方向突变,斜劈$n的$l",
	"lvl" : 80,
	"skill_name" : "苍鹰望月",
	"damage_type" : "割伤"
]),
([
	"action" : "$N陡然身行飞起，一招「满天飞雪」，手中的$w舞出万道寒光，向$n的$l掠去",
	"lvl" : 100,
	"skill_name" : "满天飞雪",
	"damage_type" : "割伤"
]),
});

int valid_enable(string usage) { return usage == "blade" || usage == "parry"; }

int valid_learn(object me)
{
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "blade")
               return notify_fail("你使用的武器不对。\n");

	if ((int)me->query("max_neili") < 50)
		return notify_fail("你的内力不够练习玄虚刀法。\n");
	if ((int)me->query_skill("yinyun-ziqi", 1) < 10)
		return notify_fail("你的氤氲紫气火候太浅。\n");
	return 1;
}

string query_skill_name(int level)
{
	int i;
	for(i = sizeof(action)-1; i >= 0; i--)
		if(level >= action[i]["lvl"])
			return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
	int i, level, j;
	level   = (int) me->query_skill("xuanxu-daofa",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"force" : 250 +random(300),
				"damage" : 100 + random(50),
				"skill_class" 	: MIAO,
				"skill_damage" 	: 20,
				"skill_attack" 	: 20,
				"skill_parry" 	: -10,
				"skill_dodge"		: 20,
				"skill_kill" 		: 20,
				"skill_rush" 		: -20,
				"skill_hurt" 	  : 0,
				"skill_GP" 			: 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("xuanxu-daofa", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 30 || me->query("neili") < 10)
		return notify_fail("你的体力不够练玄虚刀法。\n");
	me->receive_damage("jingli", 20);
	me->add("neili", -5);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"wd/xuanxu-daofa/" + action;
}
