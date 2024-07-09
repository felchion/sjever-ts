// sanhua-zhang.c -散花掌
inherit SKILL;
#include <ansi.h>
#include <skill_class.h>

string query_class() {return "柔";} 

mapping *action = ({
	([
		"action" : "$N合掌抱球，一式「"HIG"春深芳草尽"NOR"」，双掌变幻吞吐，缓缓推向$n的$l",
		"lvl" : 0,
		"skill_name" : "春深芳草尽",
		"damage_type" : "瘀伤"
	]),
	([
		"action" : "$N五指绽开，一式「"GRN"夏闲独兰馨"NOR"」，左右掌穿插翻飞，疾向$n的$l拍去",
		"lvl" : 30,
		"skill_name" : "夏闲独兰馨",
		"damage_type" : "瘀伤"
	]),
	([
		"action" : "$N使一式「"HIY"秋酣菊霜清"NOR"」，左掌虚托，右掌乍伸乍合，猛地插往$n的$l",
		"lvl" : 60,
		"skill_name" : "秋酣菊霜清",
		"damage_type" : "瘀伤"
	]),
	([
		"action" : "$N双掌血红，一式「"HIW"冬卧听梅吟"NOR"」，幻成漫天梅花朵朵，雨点般向$n击去",
		"lvl" : 90,
		"skill_name" : "冬卧听梅吟",
		"damage_type" : "瘀伤"
	])
});


int valid_enable(string usage) { return usage=="strike" || usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练散花掌必须空手。\n");
	if ((int)me->query_skill("yijin-jing", 1) < 60)
		return notify_fail("你的易筋经内功火候不够，无法学散花掌。\n");
	if ((int)me->query_int(1) > 30) {
		return 1;
	}
	else {
		if ( me->query_skill("banruo-zhang", 1)
		 && me->query_skill("yizhi-chan", 1)
		 && me->query_skill("jingang-quan", 1)
		 && me->query_skill("nianhua-zhi", 1)
		 && me->query_skill("qianye-shou", 1)
		 && me->query_skill("ruying-suixingtui", 1)
		 && me->query_skill("longzhua-gong", 1))
			return notify_fail("你的功力不够，无法同时学习两种以上的上乘功夫。\n");
	}
	if ((int)me->query("max_neili") < 600)
		return notify_fail("你的内力太弱，无法练散花掌。\n");
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
	level   = (int) me->query_skill("sanhua-zhang",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"damage_type": action[j]["damage_type"],
				"lvl": action[j]["lvl"],
				"skill_class" 	: ROU,
				"skill_damage" 	: 10 ,
				"skill_attack" 	: 20 ,
				"skill_parry" 	: 10 ,
				"skill_dodge"		: 10 ,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : 0,
				"skill_GP" 			: 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("sanhua-zhang", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 40)
		return notify_fail("你的体力太低了。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练散花掌。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"sl/sanhua-zhang/" + action;
}
