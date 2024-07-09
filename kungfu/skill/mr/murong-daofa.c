// murong-daofa.c 慕容刀法

#include <ansi.h>
#include <skill_class.h>
inherit SKILL;

string query_class() {return "正";}

mapping *action = ({
([
	"action" : "$N突然大喝一声「看招」，一招「"HIY"劈天盖地满壮志"NOR"」，刀锋自上而下向$n的$l斩去",
	"lvl" : 0,
	"skill_name" : "劈天盖地满壮志",
	"damage_type" : "割伤"
]),
([
	"action" : "$N移形换步一招「"HIR"弯刀逆转倒乾坤"NOR"」，手中$w在空中急速晃出无数光环，套向$n的$l",
	"lvl" : 30,
	"skill_name" : "弯刀逆转倒乾坤",
	"damage_type" : "割伤"
]),
([
	"action" : "$N猛一挫身，一招「"HIG"遥看万疆千里雪"NOR"」, $w从不可思议的方向冲$n的颈中斩去",
	"lvl" : 70,
	"skill_name" : "遥看万疆千里雪",
	"damage_type" : "割伤",
]),
([
	"action" : "$N身法轻盈无比，一招「"HIM"笑指银汉摘星斗"NOR"」，无数刀尖化作点点繁星，向$n的$l挑去",
	"lvl" : 100,
	"skill_name" : "笑指银汉摘星斗",
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
		return notify_fail("你的内力不够。\n");
	if ((int)me->query_skill("shenyuan-gong", 1) < 10)
		return notify_fail("你的神元功火候太浅。\n");
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
	int i, j, level;
	level = (int) me->query_skill("murong-daofa",1);

	for(i = sizeof(action); i > 0; i--) {
		if(level >= action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
			  "skill_class" 	: ZHENG,
			  "skill_damage" 	: 10,
			  "skill_attack" 	: 10,
			  "skill_parry" 	: -10,
			  "skill_dodge"		: -10,
			  "skill_kill" 		: 10,
		    "skill_rush" 		: 10,
			  "skill_hurt" 	  : 30,			
			  "skill_GP" 			: 2	
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("murong-daofa", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 40)
		return notify_fail("你的体力不够练慕容刀法。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练慕容刀法。\n");
	me->receive_damage("jingli", 30);
	me->add("neili",-10);
	return 1;
}
	   
string perform_action_file(string action)
{
	return __DIR__"mr/murong-daofa/" + action;
}
