// 穿云腿法
//written by cool

inherit SKILL;
#include <skill_class.h>

string query_class() {return "正";}  

mapping *action = ({
([
	"action" : "$N身子疾转，右脚贴地伸出，一式「划地为牢」，横扫$n的下盘",
	"damage_type" : "瘀伤",
	"lvl" : 0,
	"skill_name" : "划地为牢"
]),
([
	"action" : "$N左腿踢出，不待左腿落下，右脚紧跟着凌空踢出，一式「追风逐电」，双脚先后踢向$n的$l",
	"damage_type" : "瘀伤",
	"lvl" : 15,
	"skill_name" : "追风逐电"
]),
([
	"action" : "$N一式「左右穿花」，左脚横踹，随即向右前方落步，右脚迅速发力踢向$n的$l",
        "damage_type" : "瘀伤",
	"lvl" : 30,
	"skill_name" : "左右穿花"
]),
([
	"action" : "$N突然一个后翻，一式「倒踢北斗」，双腿晃动，向$n的$l踢去",
        "damage_type" : "瘀伤",
	"lvl" : 45,
	"skill_name" : "倒踢北斗"
]),
([
	"action" : "$N使一式「登云步月」，拔地而起，双腿连续踢出，击向$n的胸口",
        "damage_type" : "内伤",
	"lvl" : 60,
	"skill_name" : "登云步月"
]),
([
	"action" : "$N身子盘旋飞起，一式「横扫九州」，右腿横扫$n的$l",
	"damage_type" : "内伤",
	"lvl" : 75,
	"skill_name" : "横扫九州"
]),
([
	"action" : "$N左手一绕，向击向$n的头部，一式「无影无踪」，右腿突然踢向$n的$l",
	"damage_type" : "瘀伤",
	"lvl" : 90,
	"skill_name" : "无影无踪"
]),
([
	"action" : "$N使一式「雷霆天下」，身体凌空而起，双腿连环，只见漫天腿影笼罩着$n",
	"damage_type" : "震伤",
	"lvl" : 100,
	"skill_name" : "雷霆天下"
]),
});

int valid_enable(string usage) { return usage=="leg" ||  usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练穿云腿必须空手。\n");
	if ((int)me->query_skill("xuantian-wuji", 1) < 20)
		return notify_fail("你的玄天无极功火候不够，无法学穿云腿。\n");
	if ((int)me->query("max_neili") < 50)
		return notify_fail("你的内力太弱，无法练穿云腿。\n");
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
	level = (int)me->query_skill("chuanyun-tui", 1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
	      "skill_class"   : ZHENG,
	      "skill_damage"  : 0,
	      "skill_attack"  : 20,
	      "skill_parry"   : 30,
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
	int lvl = me->query_skill("chuanyun-tui", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 30)
		return notify_fail("你的体力太低了。\n");
	if ( me->query("neili") < 10)
		return notify_fail("你的内力不够练穿云腿。\n");
	me->receive_damage("jingli", 20);
	me->add("neili", -5);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"kl/chuanyun-tui/" + action;
}
