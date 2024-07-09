// huifeng-bian 回风鞭法

#include <ansi.h>

inherit SKILL;

mapping *action = ({
([
	"action":   "$N身形不动，一招「李广射石」，手中$w笔直如矢，对准$n的前胸刺去",
	"lvl" : 0,
	"skill_name" : "李广射石",
	"damage_type":  "卷伤"
]),
([
	"action":   "$N身形连转，手中$w如长龙出水，一招「长鲸吸水」将$n卷在重重鞭影当中",
	"lvl" : 20,
	"skill_name" : "长鲸吸水",
	"damage_type":  "卷伤"
]),
([
	"action":  "$N身形拔起，力贯鞭梢，一招「灵燕穿云」，手中$w舞出一道长虹直取$n的$l",
	"lvl" : 40,
	"skill_name" : "灵燕穿云",
	"damage_type":  "卷伤"
]),
([
	"action": "$N身形伏低，一招「哪吒探海」，手中$w抖出无数鞭影，对准$n的下身攻去",
	"lvl" : 60,
	"skill_name" : "哪吒探海",
	"damage_type":  "卷伤"
]),
([
	"action": "$N身形若被狂风吹送，飘然而至。一招「玉带围腰」，手中$w变出一个个圆圈，卷向$n的腰间",
	"lvl" : 80,
	"skill_name" : "玉带围腰",
	"damage_type":  "卷伤"
])
});

int valid_enable(string usage) { return (usage == "whip") || (usage == "parry"); }

int valid_learn(object me)
{
	object weapon;

	if( (int)me->query("max_neili") < 100 )
		return notify_fail("你的内力不足，没有办法练回风鞭法, 多练些内力再来吧。\n");
	if ((int)me->query_skill("huntian-qigong", 1) < 30)
		return notify_fail("你的混天气功火候太浅。\n");
	if ( !objectp(weapon = me->query_temp("weapon"))
	|| ( string)weapon->query("skill_type") != "whip" )
		return notify_fail("你必须先找一条鞭子才能练鞭法。\n");
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
	level   = (int) me->query_skill("huifeng-bian",1);
	for(i = sizeof(action); i > 0; i--)
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([  
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"force":150+random(300),
				"dodge":random(30),
				"damage":60+random(100),
				"parry":random(30),
			]);
		}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("huifeng-bian", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 40 || me->query("neili") < 20)
		return notify_fail("你的体力不够练回风鞭法。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"gb/huifeng-bian/" + action;
}
