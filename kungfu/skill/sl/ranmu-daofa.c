// ranmu-daofa.c 燃木刀法

#include <ansi.h>

inherit SKILL;

#include <skill_class.h>

string query_class() {return "刚";} 

mapping *action = ({
([
	"action" : "$N面带微笑，一招「烈火烧身」，举$w对着$n连环砍了八八六十四刀，刀气纵横，宛如烈火般扑向$p，迅雷不可挡",
	"lvl" : 0,
	"skill_name" : "烈火烧身",
	"damage_type" : "割伤"
]),
([
	"action" : "$N运起内功一招「点木生火」，在$w上带出火红的无比劲气，迅速划了一个大弧，从上而下劈向$n的$l",
	"lvl" : 30,
	"skill_name" : "点木生火",
	"damage_type" : "割伤"
]),
([
	"action" : "$N手臂一沉一拉，使出「张弓望月」，双手持着$w划出一道雪亮刀光，就在$n躲避之时猛地拦腰反切，劈向$n的$l",
	"lvl" : 60,
	"skill_name" : "张弓望月",
	"damage_type" : "割伤"
]),
([
	"action" : "$N挥舞$w上劈下撩，左挡右开，使出一招「烈火腾云」，卷起阵阵刀风，每一圈都都划出不同的气流，齐齐罩向$n",
	"lvl" : 90,
	"skill_name" : "烈火腾云",
	"damage_type" : "割伤"
]),
([
	"action" : "$N一招「火中取栗」，划个大圈后左脚跃步落地，手中$w往前直伸，刀走剑势，挟着炙热的风声点向$n的$l",
	"lvl" : 120,
	"skill_name" : "火中取栗",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N腾空而起，半空中一招「玉石俱焚」猛烈扑下，手中$w迅速连劈，满天流光般的刀影排山倒海般地向$n的全身卷去",
	"lvl" : 150,
	"skill_name" : "玉石俱焚",
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

	if ((int)me->query_skill("riyue-bian",1)
	 || (int)me->query_skill("weituo-chu",1)
	 || me->query("passdu"))
		return notify_fail("你如果有达摩老祖的修为，方可同时学习少林三绝技。\n"); 

	if(!me->query("family") || me->query("family/family_name")!="少林派" )
		return notify_fail("你不是少林弟子，如何能用这燃木刀法？\n");

	if ( me->query("max_neili") < 1500)
		return notify_fail("你的内力不够。\n");

	if ( me->query_skill("yijin-jing", 1) < 160)
		return notify_fail("你的易筋经内功火候太浅。\n");

	if ( me->query_skill("ranmu-daofa",1) <= 200
	 &&  me->query_skill("xiuluo-dao",1) <= 120)
		return notify_fail("你的修罗刀火候不够，领悟不了燃木刀。\n");
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
	level = (int) me->query_skill("ranmu-daofa",1);

	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"damage_type": action[j]["damage_type"],
				"lvl": action[j]["lvl"],
				"skill_class" 	: GANG,
				"skill_damage" 	: 20 ,
				"skill_attack" 	: 0 ,
				"skill_parry" 	: 30 ,
				"skill_dodge"		: 0 ,
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
	if ((int)me->query("jingli") < 50 )
		return notify_fail("你的精力不够练燃木刀法。\n");

	if ((int)me->query("neili") < 20 )
		return notify_fail("你的内力不够练燃木刀法。\n");

	me->receive_damage("jingli", 40);
	me->add("neili",-15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"sl/ranmu-daofa/" + action;
}

