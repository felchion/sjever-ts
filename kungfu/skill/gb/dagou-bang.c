// dagou-bang.c 打狗棒法
// Code By Looklove@SJ 2001/4/1
// Modify By Looklove 2001.4.8
// Looklove modified at 2001.5.5
// Looklove modified at 2001.5.18
// Lane@SJ modified at 2005.7.26

#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

string query_class() {return "妙";}

mapping *action = ({
([
	"action": "$N一声轻吟，挥起$w，使出一式「打草惊蛇」，猛扫向$n的$l",
	"lvl" : 0,
	"skill_name": "打草惊蛇",
	"damage_type": "挫伤"
]),
([
	"action": "$N步伐跳跃，手中$w左右晃动，一招「拨草寻蛇」向$n的$l攻去",
	"lvl":10,
	"skill_name": "拨草寻蛇",
	"damage_type": "内伤"
]),
([
	"action": "$N手中$w一圈，使出一招「关门打狗」将$n四面八方围个密不透风",
	"lvl" : 20,
	"skill_name": "关门打狗",
	"damage_type": "瘀伤"
]),
([
	"action": "$N步踏坤位，施出「拨狗朝天」，反手将$w由下往上向$n撩去",
	"lvl" : 30,
	"skill_name": "拨狗朝天",
	"damage_type": "挫伤"
]),
([
	"action": "$N大喝一声「狗急跳墙」！抢上一步，将$w劈头抡向$n的$l",
	"lvl" : 50,
	"skill_name": "狗急跳墙",
	"damage_type": "挫伤"
]),
([
	"action": "$N使出一招「棒打双犬」，手中$w化作两道青光砸向$n的$l",
	"lvl" : 80,
	"skill_name": "棒打双犬",
	"damage_type": "挫伤"
]),
([
	"action": "$N手腕一翻，一招「恶犬拦路」，将手中$w向$n的去势封了过去",
	"lvl" : 100,
	"skill_name": "恶犬拦路",
	"damage_type": "挫伤"
]),
([
	"action": "$N一招「棒打狗头」，只见$w突然飞起，直击$n的顶门",
	"lvl" : 120,
	"skill_name": "棒打狗头",
	"damage_type": "挫伤"
]),
([
	"action": "只见$N使出一招「斜打狗背」，侧身拧腰，$w斜劈向$n的$l",
	"lvl" : 140,
	"skill_name": "斜打狗背",
	"damage_type": "挫伤"
]),
([
	"action": "$N灵机一动，使出一招「反截狗臀」，反手将$w戳向$n的$l",
	"lvl" : 160,
	"skill_name": "反截狗臀",
	"damage_type": "挫伤"
]),
([
	"action": "$N一招「打狗入洞」，将$w舞了个棒花，横扫$n的$l",
	"lvl" : 180,
	"skill_name": "打狗入洞",
	"damage_type": "挫伤"
]),
([
	"action": "$N反身错步，手中$w一幌，使一招「压肩狗背」，夹头夹脸打向$n的$l",
	"lvl" : 200,
	"skill_name": "压肩狗背",
	"damage_type": "内伤"
]),
});

int valid_enable(string usage) { return (usage == "stick") || (usage == "parry"); }

int valid_learn(object me)
{
	if( me->query("family/master_name") != "洪七公" )
		return notify_fail("打狗棒法只能从洪七公处学到。\n");
	if( me->query_skill("stick", 1) < 20 )
		return notify_fail("你的基本棒法太差, 还领会不了打狗棒法。\n");
	if( me->query("gender") == "无性" )
		return notify_fail("你一个不男不女之人如何能学打狗棒？\n");
	if( me->query("max_neili") < 100 )
		return notify_fail("你的内力不够。\n");
	return 1;
}

string query_skill_name(int level)
{
	int i = sizeof(action);
	while (i--)
		if( level > action[i]["lvl"] )
			return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
	int i, j,level,bonus;
	bonus = (me->query("gender")=="女性")?20:0;
  
  level = (int)me->query_skill("dagou-bang", 1);
	for(i = sizeof(action); i > 0; i--)
		if(level >= action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action" : action[j]["action"],
				"lvl" : action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"skill_class" 	: MIAO,
				"skill_damage" 	: 0 + bonus,
				"skill_attack" 	: 10 + bonus,
				"skill_parry" 	: 20 + bonus ,
				"skill_dodge"		: 20 + bonus,
				"skill_kill" 		: 0 - bonus,
				"skill_rush" 		: 0 - bonus,
				"skill_hurt" 	  : 0 - bonus,
				"skill_GP" 			: 2
			]);
		}
}

int practice_skill(object me)
{
	object weapon;
	int lvl = me->query_skill("dagou-bang", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if( !objectp(weapon = me->query_temp("weapon"))
	 ||(string)weapon->query("skill_type") != "stick" )
		return notify_fail("你使用的武器不对。\n");

	if( me->query("jingli") < 50 )
		return notify_fail("你的精力不够练打狗棒法。\n");

	if( me->query("neili") < 20 )
		return notify_fail("你的内力不够练打狗棒。\n");

	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"gb/dagou-bang/" + action;
}


int help(object me)
{
        write(HIG"\n打狗棒："NOR"\n");
        write(@HELP
        打狗棒共有绊、劈、缠、戳、挑、引、封、转八诀，是闻名天下的
        丐帮镇帮绝技。以轻、灵、奇、巧诸称，舞将起来是四处只见棒影
        不见人影。武学中有言道：“四两拨千斤”，乃打狗棒法的精妙招
        数精髓体现。修习时如不同时学习棒诀，终将无法大成。

HELP
        );
        return 1;
}

                                                                                      