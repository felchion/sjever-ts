// fanliangyi-dao.c 反两仪刀

inherit SKILL;
#include <skill_class.h>

string query_class() {return "奇";}  

mapping *action = ({
([
	"action" : "$N揉身而上至“随”位，刀交左手反劈「日月晦明」，幻起无数刀光狂削，令$n看不清来势！",
	"lvl" : 0,
	"skill_name" : "日月晦明",
	"damage_type" : "割伤"
]),
([
	"action" : "$N反踏两仪方位满场游走，一招「万劫不复」连划数弧，$w激起刺骨寒风，将$n裹入刀气的旋涡中！",
	"lvl" : 15,
	"skill_name" : "万劫不复",
	"damage_type" : "割伤"
]),
([
	"action" : "$N使出「无色无相」旋身到“归妹”，左掌劲气弥漫直拍而出，右手$w却无声无息挥至$n的$l！",
	"lvl" : 35,
	"skill_name" : "无色无相",
	"damage_type" : "割伤"
]),
([
	"action" : "$N身形闪动抢达“明夷”，$w一式「太乙生萌」，凌历无俦地对准$n的$l劈砍下去！",
	"lvl" : 50,
	"skill_name" : "太乙生萌",
	"damage_type" : "割伤"
]),
([
	"action" : "$N向前跨上一步，稳据“无妄”使出「两仪合德」，挥动$w向$n左右闪电般连斩两刀！",
	"lvl" : 65,
	"skill_name" : "两仪合德",
	"damage_type" : "割伤"
]),
([
	"action" : "$N拧身急转施展「混沌一破」，猛然挥刀从背后反划了个弧形，弯弯曲曲地斩向$n$l！",
	"lvl" : 80,
	"skill_name" : "混沌一破",
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
	if ((int)me->query_skill("zixia-gong", 1) < 10)
		return notify_fail("你的紫霞神功火候太浅。\n");
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
	level   = (int) me->query_skill("fanliangyi-dao",1);
	for(i = sizeof(action); i > 0; i--) {
                if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([  
				"action":	action[j]["action"],
				"lvl":		action[j]["lvl"],
				"damage_type":	action[j]["damage_type"],
		    "skill_class"   : QI,
		    "skill_damage"  : 30,
		    "skill_attack"  : 0,
		    "skill_parry"   : 40,
		    "skill_dodge"   : -20,
		    "skill_kill"    : 0,
		    "skill_rush"    :-50,
		    "skill_hurt"    : 50,
		    "skill_improve" : 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("fanliangyi-dao", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 40)
		return notify_fail("你的精力不够练反两仪刀。\n");
	if ((int)me->query("jingli") < 20)
		return notify_fail("你的内力不够练反两仪刀。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"hs/fanliangyi-dao/" + action;
}
