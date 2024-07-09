// poyu-quan.c -劈石破玉拳

inherit SKILL;
#include <skill_class.h>

string query_class() {return "刚";}  
mapping *action = ({
([
	"action" : "$N一式「起手式」，身体右甩，屈肘反切，双拳蓄势而发，击向$n的$l",
	"lvl" : 0,
	"skill_name" : "起手式",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N一式「石破天惊」，左掌向上，右掌向下，拳风吡啪爆响，一股劲力直冲$n的$l",
	"lvl" : 10,
	"skill_name" : "石破惊天",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N全身提气，腾空飞起，一式「铁闩横门」，双拳双腿齐出，来势汹汹，令$n无可躲藏",
	"lvl" : 20,
	"skill_name" : "铁闩横门",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N神情凝重，双掌虚含，掌缘下沉，大喝一声，一式「千斤坠地」，缓缓向$n推出",
	"lvl" : 30,
	"skill_name" : "千斤坠地",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N一臂前伸，一臂后指，一式「傍花拂柳」，身行急闪直$n身前，攻向$n的$l",
	"lvl" : 40,
	"skill_name" : "傍花拂柳",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N一式「金刚挚尾」，右拳由下至上，左拳从左到右，迅雷不及掩耳之势双双击向$n的$l",
	"lvl" : 50,
	"skill_name" : "金刚挚尾",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N两目内视，双手内笼，一式「封闭手」，双拳打向$n，只见$n躲闪过去，又击向$n的$l",
	"lvl" : 60,
	"skill_name" : "封闭手",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N调整内息，紧握双拳，一式「粉石碎玉」，全身发出暴豆般的响声，用尽全身力量击向$n的$l",
	"lvl" : 80,
	"skill_name" : "粉石碎玉",
	"damage_type" : "瘀伤"
])
});

int valid_enable(string usage) { return  usage=="cuff" || usage=="parry"; }

int valid_learn(object me)
{
	if ( me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练劈石破玉拳须空手。\n");
	if ( me->query_skill("zixia-gong", 1) < 10)
		return notify_fail("你的紫霞神功不够，无法学劈石破玉拳。\n");
	if ( me->query("max_neili") < 50)
		return notify_fail("你的内力太弱，无法练劈石破玉拳。\n");
	return 1;
}

mapping query_action(object me, object weapon)
{
	int i, level, j;
	level = me->query_skill("poyu-quan",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
		    "skill_class"   : GANG,
		    "skill_damage"  : 30,
		    "skill_attack"  : 0,
		    "skill_parry"   : 20,
		    "skill_dodge"   : 0,
		    "skill_kill"    : 0,
		    "skill_rush"    : 0,
		    "skill_hurt"    : 0,
		    "skill_improve" : 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("poyu-quan", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 40)
		return notify_fail("你的体力太低了。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练劈石破玉拳。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

string perform_action_file(string action)
{
        return __DIR__"hs/poyu-quan/" + action;
}
