// zui-gun.c -少林醉棍

#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

string query_class() {return "慢";}

mapping *action = ({
([
	"action":"「蓝采和，提篮劝酒醉朦胧」，$N手中$w半提，脚下蹒跚，缓缓划向$n的$l",
	"lvl" : 0,
	"skill_name" : "蓝采和，提篮劝酒醉朦胧",
	"damage_type":"挫伤"
]),
([
	"action":"「何仙姑，拦腰敬酒醉仙步」，$N摆姿扭腰，右臂挟$w顺势猛地扫向$n的$l",
	"lvl" : 10,
	"skill_name" : "何仙姑，拦腰敬酒醉仙步",
	"damage_type":"挫伤"
]),
([
	"action":"「曹国舅，千杯不醉倒金盅」，$N往前一晃，倒竖$w起，指天打地，向$n的$l劈去",
	"lvl" : 20,
	"skill_name" : "曹国舅，千杯不醉倒金盅",
	"damage_type":"挫伤"
]),
([
	"action":"「韩湘子，铁棍提胸醉拔萧」，$N摇头晃脑，横提$w，棍端划了个半圈，击向$n的$l",
	"lvl" : 40,
	"skill_name" : "韩湘子，铁棍提胸醉拔萧",
	"damage_type":"挫伤"
]),
([
	"action":"「汉钟离，跌步翻身醉盘龙」，$N手中棍花团团，脚步也摇摇摆摆，扫起疾风般向卷向$n",
	"lvl" : 60,
	"skill_name" : "汉钟离，跌步翻身醉盘龙",
	"damage_type":"挫伤"
]),
([
	"action":"「铁拐李，踢倒金山醉玉池」，$N单腿支地，另一腿踢在$w根部荡起，甩向$n的$l",
	"lvl" : 90,
	"skill_name" : "铁拐李，踢倒金山醉玉池",
	"damage_type":"挫伤"
]),
([
	"action":"「张果老，醉酒抛杯倒骑驴」，$N扭身反背，摇摇欲坠，$w从胯底钻出，戳向$n的胸口",
	"lvl" : 120,
	"skill_name" : "张果老，醉酒抛杯倒骑驴",
	"damage_type":"挫伤"
]),
([
	"action":"「吕洞宾，酒醉提壶力千钧」，$N醉眼朦胧，但却举重若轻，手中$w挟千钧之力压向$n",
	"lvl" : 150,
	"skill_name" : "吕洞宾，酒醉提壶力千钧",
	"damage_type":"挫伤"
]),
});


int valid_enable(string usage) { return (usage == "club") || (usage == "parry"); }

int valid_learn(object me)
{
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "club")
		return notify_fail("你使用的武器不对。\n");

	if ((int)me->query("max_neili") < 800)
		return notify_fail("你的内力不够。\n");
	if ((int)me->query_skill("yijin-jing", 1) < 70)
		return notify_fail("你的易筋经内功火候太浅。\n");
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
	level   = (int) me->query_skill("zui-gun",1);
	
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"damage_type": action[j]["damage_type"],
				"lvl": action[j]["lvl"],
				"skill_class" 	: MAN,
				"skill_damage" 	: 20 ,
				"skill_attack" 	: -10 ,
				"skill_parry" 	: 30 ,
				"skill_dodge"		: -10 ,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : 50,
				"skill_GP" 			: 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("zui-gun", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 40 || me->query("neili") < 20)
		return notify_fail("你的体力不够练少林醉棍。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"sl/zui-gun/" + action;
}
