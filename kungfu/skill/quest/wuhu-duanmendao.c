// wuhu-duanmendao.c 五虎断门刀
// Modified By Numa@SJ 2000.7

#include <ansi.h>
#include <skill_class.h>
inherit SKILL;

int query_ap() {return 3;}

string query_class() {return "正";}

mapping *action = ({
([
	"action" : "$N将手中$w向右一立，随即吸气用力一纵，以「猛虎下山」之势扑向$n",
	"damage_type" : "割伤",
	"lvl" : 0,
	"skill_name" : "猛虎下山"
]),
([
	"action" : "$N一招「西风倒卷」，手中$w由下拖上直直划过$n小腹",
	"damage_type" : "割伤",
	"lvl" : 40,
	"skill_name" : "西风倒卷"
]),
([
	"action" : "$N身体由左至右旋转一周，手中$w则以一招「飞沙走石」扬向$n",
	"damage_type" : "割伤",
	"lvl" : 80,
	"skill_name" : "飞沙走石"
]),
([
	"action" : "$N全身运力一跃，约莫丈许高时急速下窜，一招「天昏地暗」直取$n顶门",
	"damage_type" : "割伤",
	"lvl" : 120,
	"skill_name" : "天昏地暗"
]),
([
	"action" : "$N蹲了一个马步，劲力皆运于$w之上，以一招「横扫天下」直逼$n双膝",
	"damage_type" : "割伤",
	"lvl" : 150,
	"skill_name" : "横扫天下"
]),
([
	"action" : "$N口中突然暴喝，左手画了个圆圈，右手以刀柄抵住手掌，一招「气吞山河」向$n猛压下去",
	"damage_type" : "割伤",
	"lvl" : 200,
	"skill_name" : "气吞山河"
]),
});

int valid_enable(string usage) {return (usage == "blade") || (usage == "parry");}

int valid_learn(object me)
{
	object weapon;

	if ( me->query_skill("wuhu-duanmendao", 1) < 150)
		return notify_fail("「五虎断门刀」绝学只能从秘籍中领悟。\n");
	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "blade")
		return notify_fail("你使用的武器不对。\n");
	if ((int)me->query("max_neili") < 300)
		return notify_fail("你的内力不够。\n");
	if ((int)me->query_skill("blade", 1) < 30)
		return notify_fail("你的基本刀法火候不够。\n");
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
	int i, level, j,bonus;
	level   = (int) me->query_skill("wuhu-duanmendao",1);

	if (me->query("mao18/pass2") && me->query("mao18/pass3"))
		bonus = 20;
	
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action"				:	action[j]["action"],
				"lvl"						:	action[j]["lvl"],
				"force"					: 350 +random(150),
				"damage_type"		:	action[j]["damage_type"],
				"skill_class" 	: ZHENG,
				"skill_damage" 	: 20 + bonus,
				"skill_attack" 	: 10,
				"skill_parry" 	: 0,
				"skill_dodge"		: 0,
				"skill_kill" 		: 10,
				"skill_rush" 		: bonus ,
				"skill_hurt" 	  : 30 + bonus,			
				"skill_GP" 			: 2		
			]);
		}
	}
}

int practice_skill(object me)
{
	if ( me->query("jingli") < 50)
		return notify_fail("你的体力不够练五虎断门刀法。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练五虎断门刀法。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"quest/wuhu-duanmendao/" + action;
}
