// tiezhang-zhangfa 铁掌掌法
// BY BOBO@SJ 
inherit SKILL;
#include <ansi.h>
#include <skill_class.h>
string query_class() {return "妙";}

mapping *action = ({
([
	"action": "$N右掌一拂而起，一招「推窗望月」，左掌自侧面连消带打，登时将$n的力道带斜，右掌一挥，拍向$p$l",
	"lvl": 0,
	"damage_type": "瘀伤",
	"skill_name": "推窗望月"
]),
([
	"action": "$N使一招「分水擒龙」，右臂突伸抓向$n，左掌陡然沿着伸长的右臂，飞快的一削而出，斩向$n的$l",
	"lvl": 20,
	"damage_type": "瘀伤",
	"skill_name": "分水擒龙"
]),
([
	"action": "$N突然使一式「白云幻舞」，双臂如旋风一般一阵狂舞，刮起一阵旋转的气浪，掌势一推，压向$n",
	"lvl": 40,
	"damage_type": "瘀伤",
	"skill_name": "白云幻舞"
]),
([
	"action": "$N一招「掌中乾坤」，一掌拍出以后，猛地侧过身来，右臂自左肋下疾翻而出，拇，中两指扣圈猛弹$n的$l",
	"lvl": 60,
	"damage_type": "内伤",
	"skill_name": "掌中乾坤"
]),
([
	"action": "$N一招「落日赶月」，伸掌一拍一收，一拍无丝毫力道，一收之间，一股阴柔无比的力道才陡然发出",
	"lvl": 80,
	"damage_type": "瘀伤",
	"skill_name": "落日赶月"
]),
([
	"action": "$N身行暴起，一式「蛰雷为动」，双掌横横切出，掌缘才递出，呜呜呼啸之声狂作，铁掌之名，由此而出",
	"lvl": 100,
	"damage_type": "内伤",
	"skill_name": "蛰雷为动"
]),
([
	"action": "$N一招「天罗地网」，左掌大圈而出，右掌小圈而出，两股奇异的力道一会之下，巨大的内劲压向$n的$l",
	"lvl": 120,
	"damage_type": "瘀伤",
	"skill_name": "天罗地网"
]),
([
	"action": "$N一招「五指幻山」，猛一吐气，单掌有如推门，另一掌却是迅疾无比的一推即收，双劲合一，去势如虎",
	"lvl": 140,
	"damage_type": "内伤",
	"skill_name": "五指幻山"
]),
});

int valid_enable(string usage) { return usage=="strike" || usage=="parry"; }

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

	level = me->query_skill("tiezhang-zhangfa",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level >= action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);      
			return ([  
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"skill_class"   : MIAO,
				"skill_damage"  : 10,
				"skill_attack"  : 30,
				"skill_parry"   : 10,
				"skill_dodge"   : 20,
				"skill_kill"    : 0,
				"skill_rush"    : 10,
				"skill_hurt"    : 0,
				"skill_GP"      : 2
			]);
		}
	}
}

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练铁掌掌法必须空手。\n");
	if ((int)me->query_skill("guiyuan-tunafa", 1) < 15)
		return notify_fail("你的归元吐呐法火候不够，无法练铁掌掌法。\n");
	if ((int)me->query("max_neili") < 40)
		return notify_fail("你的内力太弱，无法练铁掌掌法");
	return 1;
}

int practice_skill(object me)
{
	int lvl = me->query_skill("tiezhang-zhangfa", 1);
	int i = sizeof(action);
	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 50)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练铁掌掌法。\n");

	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"tz/tiezhang-zhangfa/" + action;
}
