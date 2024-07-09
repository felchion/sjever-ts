#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

string query_class() {return "慢";}


mapping *action = ({
([
	"action": HIG"$N将$w抖动成圆，一式「圆转如意」，$w已将$n$l缠住"NOR,
	"lvl" : 0,
	"skill_name" : "圆转如意",
	"damage_type":  "拉伤"
]),
([
	"action": HIW"$N的$w在$n身旁连连进击，忽然一招「银龙回首」从$n$l处倒卷上来"NOR,
	"lvl" : 60,
	"skill_name" : "银龙回首",
	"damage_type":  "拉伤"
]),
([
	"action": HIM"$N运起九阴真气，$w陡然变成笔直，一式「银龙出洞」，向$n直刺过去"NOR,
	"lvl" : 90,
	"skill_name" : "银龙出洞",
	"damage_type":  "刺伤"
]),
});

int valid_enable(string usage) { return (usage == "whip") || (usage == "parry"); }

int valid_learn(object me)
{
	object weapon;
	if( !me->query("quest/jiuyin2/pass") && !me->query("quest/jiuyin1/pass"))
		return notify_fail("你不可练习九阴银龙鞭。\n");
	if( (int)me->query("max_neili") < 1000 )
		return notify_fail("你的内力修为不足，没有办法练九阴银龙鞭。\n");
	if ( me->query_skill("yinlong-bian", 1) < 150 )
		return notify_fail("你的九阴银龙鞭等级还不够。\n");
//	if ((int)me->query_skill("jiuyin-zhengong", 1) <= (int)me->query_skill("yinlong-bian", 1))
//		return notify_fail("你的九阴真功火候太浅。\n");
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

mapping query_action(object me)
{
	int i, level, j;
	level   = (int) me->query_skill("yinlong-bian",1);
	for(i = sizeof(action); i > 0; i--)
                if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action"				:	action[j]["action"],
				"lvl"						:	action[j]["lvl"],
				"damage_type"		:	action[j]["damage_type"],
				"skill_class" 	: MAN,
				"skill_damage" 	: 20,
				"skill_attack" 	: 50,
				"skill_parry" 	: 20,
				"skill_dodge"		: 50,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : 0,			
				"skill_GP" 			: 2		
			]);
		}
}

int practice_skill(object me)
{
	if ((int)me->query("jingli") < 60)
		return notify_fail("你的体力不够练九阴银龙鞭。\n");
	if ((int)me->query("neili") < 30 )
		return notify_fail("你的内力不够练九阴银龙鞭。\n");
	me->receive_damage("jingli", 50);
	me->add("neili",-20);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"quest/yinlong-bian/" + action;
}
