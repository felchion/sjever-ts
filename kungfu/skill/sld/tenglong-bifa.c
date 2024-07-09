// canglang-goufa.c 沧浪钩法
// by aug   99.9.10
// 换个名字，而且sld的钩子好象也不怎么出名
// 改成dagger功夫，名字，olives定的
// tenglong-dagger 腾龙匕法????我怎么越听觉得越土......
// modified by Olives@SJ 4/18/2001
// 这个功夫将加上6个pfm
// 就是所谓的美人三招，英雄三式
// 分别在不同的情况下用不同的pfm

inherit SKILL;
#include <ansi.h>
#include <skill_class.h>
string query_class() {return "险";}

mapping *action = ({
([
	"action":   "$N身形不动，一招「飞龙夺珠」，手中$w直奔$n的双目而去",
	"lvl" : 0,
	"skill_name" : "飞龙夺珠",
	"damage_type" : "刺伤"
]),
([
	"action":   "$N一招「攀龙附凤」，手中$w如影附形，向$n的$l刺去",
	"lvl" : 40,
	"skill_name" : "攀龙附凤",
	"damage_type" : "刺伤"
]),
([
	"action": "$N猛地俯身前冲，一招「金龙出海」，手中$w直撩而上，直奔$n的头部而去",
	"lvl" : 80,
	"skill_name" : "金龙出海",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N一反身，使一招「龙腾虎跃」，$w不顾一切的劈向$n的$l",
	"lvl" : 100,
	"skill_name" : "殊途同归",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N右手$w向后撤，再向前平平托起，一招「描龙绣凤」刺向$n的$l",
	"lvl" : 140,
	"skill_name" : "描龙绣凤",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N一转身，$w向后斜带，一招「游龙归巢」，砍向$n的$l",
	"lvl" : 160,
	"skill_name" : "游龙归巢",
	"damage_type" : "割伤"
]),
([
	"action" : "$N右脚向前一步，左膝提起，一招「游龙戏凤」，$w向前上方刺向$n的$l",
	"lvl" : 180,
	"skill_name" : "游龙戏凤",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N身形晃动，猱身而上，切入$n的怀中，一招「龙翔凤舞」手中$w狠狠刺向$n的胸口。",
	"lvl" : 200,
	"skill_name" : "龙翔凤舞",
	"damage_type" : "割伤",
	"post_action" : (: call_other, __FILE__, "post_action" :),
]),
});

int valid_enable(string usage) { return usage == "dagger" || usage == "parry"; }

int valid_learn(object me)
{
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "dagger")
		return notify_fail("你使用的武器不对。\n");
	if ( me->query("max_neili") < 1000)
		return notify_fail("你的内力不够。\n");
	if ( me->query_skill("dulong-dafa", 1) < 100)
		return notify_fail("你的毒龙大法火候太浅，无法学习腾龙匕法。\n");
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
	int i, level, j, bonus;
  if (me->query("gender")=="女性") bonus=20;	
	level = me->query_skill("tenglong-bifa",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"skill_class"   : XIAN,
				"skill_damage"  : 10+bonus,
				"skill_attack"  : bonus,
				"skill_parry"   : -20-bonus,
				"skill_dodge"   : 20,
				"skill_kill"    : 100-5*bonus,
				"skill_rush"    : 20,
				"skill_hurt"    : 0+5*bonus,
				"skill_GP"      : 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("tenglong-bifa", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 50)
		return notify_fail("你的精力不够练腾龙匕法。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练腾龙匕法。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"sld/tenglong-bifa/" + action;
}
