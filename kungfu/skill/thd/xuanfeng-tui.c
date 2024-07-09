//    旋风扫叶腿
#include <skill_class.h>
inherit SKILL;

string query_class() {return "快";}

mapping *action = ({
([
	"action" : "$N纵身向前，忽然伸出左腿，一式「魁星踢斗」，直踢$n的头部",
	"lvl" : 0,
	"skill_name" : "魁星踢斗",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N身形一闪，已从$n身边掠过，右足反踢，正是一招「倒踢紫金冠」，踹向$n的背心",
	"lvl" : 10,
	"skill_name" : "倒踢紫金冠",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N身体半蹲，右足画出个半圆，使出「旁敲侧击」，扫向$n的膝部",
	"lvl" : 20,
	"skill_name" : "旁敲侧击",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N上身微侧，左足倏地弹出，连环三腿，分踢$n的头部，胸部和裆部，正是「连环三踢」",
	"lvl" : 40,
	"skill_name" : "连环三踢",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N右足独立，左足随身形反转横扫，又左足立定，右足横扫向前，一招「秋风扫落叶」，踢向$n的$l",
	"lvl" : 80,
	"skill_name" : "秋风扫落叶",
	"damage_type" : "瘀伤"
]),
});

int valid_enable(string usage) { return usage=="leg" || usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("学旋风扫叶腿时手里不能拿武器。\n");
	if ((int)me->query_skill("bihai-chaosheng", 1) < 30)
		return notify_fail("你的碧海潮生功火候不够，无法学旋风扫叶腿。\n");
	if (me->query_skill_mapped("force") != "bihai-chaosheng")
		return notify_fail("修习旋风扫叶腿必须有碧海潮生功配合。\n");
	if ((int)me->query("max_neili") < 100)
		return notify_fail("你的内力太弱，无法练旋风扫叶腿。\n");
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
	int i, level, j,bonus=0;
	if (me->query("gender")=="男性") bonus=10;
	level   = (int) me->query_skill("xuanfeng-tui",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
			  "skill_class" 	: KUAI,
				"skill_damage" 	: -20+bonus,
				"skill_attack" 	: 20-bonus,
				"skill_parry" 	: 10,
				"skill_dodge"		: 10,
				"skill_kill" 		: 0,
				"skill_rush" 		: 50,
				"skill_hurt" 	  : bonus,
				"skill_GP" 			: 2	
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("xuanfeng-tui", 1);
	int i = sizeof(action);
	mapping learned = me->query_learned();

	if (!mapp(learned)) learned = ([]);
	if (!learned["xuanfeng-tui"])
		while (i--)
			if (lvl == action[i]["lvl"])
				return notify_fail("你将已经学会的所有招式练习了一遍，发现如果不去找师父学新的招式就无法获得进步了。\n");;

	if ((int)me->query("jingli") < 30)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 10)
		return notify_fail("你的内力不够练旋风扫叶腿。\n");
	me->receive_damage("jingli", 20);
	me->add("neili", -5);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"thd/xuanfeng-tui/" + action;
}
