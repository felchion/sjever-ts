//    /kungfu/skill/xingyi-zhang.c 星移掌
// by leontt /1/5/2000

#include <ansi.h>
#include <skill_class.h>
inherit SKILL;

string query_class() {return "奇";}

mapping *action = ({
([
	"action" : "$N使一招「幻形变影」，双手划了个半圈，按向$n的$l",
	"lvl" : 0,
	"skill_name" : "幻形变影",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N左手虚按，右手使一招「星星点点」，向$n的$l推去",
	"lvl" : 20,
	"skill_name" : "星星点点",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N双手握拳，向前向后划弧，一招「漫天星斗」打向$n的$l",
	"lvl" : 40,
	"skill_name" : "漫天星斗",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N左手虚划，右手一记「五斗星辰」击向$n的腰部",
	"lvl" : 60,
	"skill_name" : "五斗星辰",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N施出「北斗七星」，右手击向$n的$l，左手攻向$n的裆部",
	"lvl" : 80,
	"skill_name" : "北斗七星",
	"damage_type" : "瘀伤"
]),						   
([
	"action" : "$N跃起在半空，双掌卷起数道罡风，一式「追星赶月」迅捷无比地劈向$n",
	"lvl" : 100,
	"skill_name" : "追星赶月",
	"damage_type" : "瘀伤"
]),
});

int valid_enable(string usage) { return usage=="strike" || usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练斗转星移掌必须空手。\n");
	if ( me->query_skill("shenyuan-gong", 1) < 15)
		return notify_fail("你的神元功火候不够，无法学斗转星移掌。\n");
	if ( me->query("max_neili") < 100)
		return notify_fail("你的内力太弱，无法练斗转星移掌。\n");
	return 1;
}

mapping query_action(object me, object weapon)
{
	int i, level, j;
	level   = (int) me->query_skill("xingyi-zhang",1);

	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
				"force": 150 + random(300),
				"skill_class" 	: QI,
	    	"skill_damage" 	: 30,
			  "skill_attack" 	: 0,
			  "skill_parry" 	: 0,
			  "skill_dodge"		: 0,
			  "skill_kill" 		: 0,
		    "skill_rush" 		: 0,
			  "skill_hurt" 	  : 20,			
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("xingyi-zhang", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 40)
		return notify_fail("你的体力太低了。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练星移掌。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"mr/xingyi-zhang/" + action;
}
