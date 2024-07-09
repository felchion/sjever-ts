//xunlei-jian by cool
#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

string query_class() {return "快";}  

mapping *action = ({
([
	"action" : "$N剑尖向右，绕身一周，一式「仙人指路」，$w突然向$n的$l刺去",
        "lvl" : 0,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N使出一式「鹞子翻身」，身体凌空侧翻，一剑从身下刺出",
        "lvl" : 5,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N左手剑指血指，右手$w使出一招「海底寻针」，由上至下猛向$n的$l劈刺",
        "lvl" : 10,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N撤步缩身，$w暗藏于臂下，一招「灵猿探洞」，快如闪电般刺向$n的$l",
        "lvl" : 15,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N踏步向前，一式「拨草寻蛇」，手中长剑摆动，剑尖刺向$n的$l",
        "lvl" : 20,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N一招「斜插神枝」，身体背转，手中$w由右肩上方反手向下刺出",
        "lvl" : 25,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N一式「电闪雷动」，剑走中锋，气势威严，将$n笼罩于重重剑气之中",
        "lvl" : 30,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N向前弯身，一招「夫子揖手」，$w忽然从身下刺出，快如流星闪电",
        "lvl" : 35,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N横握$w，左右晃动，一招「玉带缠腰」，剑气直逼$n的腰部要害",
        "lvl" : 45,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N双手持剑，将$w当做刀使，一招「举火烧天」，由身后向$n的前上方劈去",
        "lvl" : 55,
	"damage_type" : "劈伤"
]),
([
	"action" : "$N侧身向$n，使出一招「败马斩蹄」，挥动手中$w，直劈$n的下三路",
        "lvl" : 65,
	"damage_type" : "劈伤"
]),
([
	"action" : "$N手中$w由右自左，一招「玉女穿针」，$w突然反手刺向$n的$l",
        "lvl" : 75,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N跳步向前，剑尖上指，一招「灵猿登枝」，$w挑向$n的头部要害",
        "lvl" : 85,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N一招「苏武挥鞭」，$w随身走，犹如一条白龙，将$n全身上下笼罩",
        "lvl" : 95,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N剑尖向下，一招「挑灯看剑」，$w忽然急转直上，剑气将$n的上身要害团团围住",
        "lvl" : 110,
	"damage_type" : "刺伤"
]),
});

int valid_enable(string usage) { return (usage == "sword") || (usage == "parry"); }

int valid_learn(object me)
{
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "sword")
		return notify_fail("你使用的武器不对。\n");
	if ((int)me->query("max_neili") < 100)
		return notify_fail("你的内力不够。\n");
	if ((int)me->query_skill("xuantian-wuji", 1) < 30)
		return notify_fail("你的玄天无极功火候太浅。\n");
	if ((int)me->query_skill("sword", 1) < 20)
		return notify_fail("你的基本剑法火候太浅。\n");
	return 1;
}

mapping query_action(object me, object weapon)
{
	int i, level, j;
	level   = (int) me->query_skill("xunlei-jian",1);

	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
	      "skill_class"   : KUAI,
	      "skill_damage"  : 10,
	      "skill_attack"  : 30,
	      "skill_parry"   : -20,
	      "skill_dodge"   : 20,
	      "skill_kill"    : 30,
	      "skill_rush"    : 30,
	      "skill_hurt"    : -40,
	      "skill_improve" : 2
			]);			
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("xunlei-jian", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 40)
		return notify_fail("你的体力不够练迅雷剑法。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练迅雷剑法。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"kl/xunlei-jian/" + action;
}
