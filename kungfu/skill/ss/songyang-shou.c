// songyang-shou.c  嵩阳手
// Created by Numa 2000.1.5
// Modified by Numa 2000.1.25
// Modified by darken@SJ
// Modified by Looklove@SJ 2001.10.22

#include <skill_class.h>
#include <ansi.h>
inherit SKILL;


string query_class() {return "奇";}

mapping *action = ({
([
	"action" : "$N一掌高，一掌低，摆出了“嵩阳手”的起首式--「嵩阳拜首」",
	"damage_type" : "瘀伤",
	"lvl" : 0,
	"skill_name" : "嵩阳拜首"
]),
([
	"action" : "$N左掌一提，右掌一招便即劈出。但见他一掌出手，登时全身犹如渊停岳峙，气度凝重",
	"damage_type" : "内伤",
	"lvl" : 30,
	"skill_name" : "渊停岳峙"
]),
([
	"action" : "$N大喝一声道：“小心了！”随即双掌凌空推出，一股猛烈的掌风向着$n逼体而至",
	"damage_type" : "瘀伤",
	"lvl" : 60,
	"skill_name" : "中岳迎风"
]),
([
	"action" : "$N两手连续挥舞，双掌交错，一拍$n的面门，一拍$n的小腹",
	"damage_type" : "瘀伤",
	"lvl" : 80,
	"skill_name" : "松柏苍天",
]),
([
	"action" : "$N踏前一步，绕$n坻壁而行，双手快如风、迅如电，不停击向$n周身各大要穴",
	"lvl" : 100,
	"damage_type" : "瘀伤",
	"skill_name" : "坻壁而行"
]),
([
	"action" : "$N运气丹田，连续几个斜步围绕$n周身不断出掌，其迅捷令$n全身各大要穴均暴露无疑",
	"damage_type" : "内伤",
	"lvl" : 120,
	"skill_name" : "岳雾缭人"
]),
([
	"action" : "$N蹂身向前，身行猛的一窜，窜至半空又两手互抵，双掌竖直向$n的天门压去",
	"damage_type" : "内伤",
	"lvl" : 140,
	"skill_name" : "临渊下探"
]),
});

int valid_enable(string usage) { return usage=="hand" ||  usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("学习嵩阳手必须空手。\n");
	if ((int)me->query_skill("hanbing-zhenqi", 1) < 20)
		return notify_fail("你的寒冰真气火候不够，无法学嵩阳手。\n");
	if ((int)me->query("max_neili") < 100)
		return notify_fail("你的内力太弱，无法练嵩阳手。\n");
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
	level = (int)me->query_skill("songyang-shou", 1);

	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"], 
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
				"skill_class" 	: QI,
				"skill_damage" 	: 10,
				"skill_attack" 	: 20,
				"skill_parry" 	: 30,
				"skill_dodge"		: 0,
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
	int lvl = me->query_skill("songyang-shou", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if (me->query("jingli") < 40)
		return notify_fail("你的精力太低了。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练嵩阳手。\n");
	me->receive_damage("jingli", 30);
	me->receive_damage("neili", 10);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"ss/songyang-shou/" + action;
}
