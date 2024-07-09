// pangen-fu.c 盘根错结斧

inherit SKILL;
#include <ansi.h>
#include <skill_class.h>
string query_class() {return "刚";}
mapping *action = ({
([
	"action" : "$N高举手中$w，一式「力劈华山」，纵身上前，由上到下，劈向$n的$l",
	"lvl" : 0,
	"skill_name" : "力劈华山",
	"damage_type" : "割伤"
]),
([
	"action" : "$N向左一个滑步，一招「高山流水」，出其不意转到$n身后，$w从上到下砍了下去",
	"lvl" : 30,
	"skill_name" : "高山流水",
	"damage_type" : "割伤"
]),
([
	"action" : "$N挥动$w，幻化出无数个$w，一招「幻影流动」，刺向$n的死穴",
	"lvl" : 40,
	"skill_name" : "幻影流动",
	"damage_type" : "割伤"
]),
([
	"action" : "$N一招「震山式」，倒转$w, 大开大阖，高举头顶,直砸向$n",
	"lvl" : 60,
	"skill_name" : "震山式",
	"damage_type" : "内伤"
]),
([
	"action" : "$N手中$w一荡，一招「枯树盘根」，双手横握$w扫向$n的下盘",
	"lvl" : 90,
	"skill_name" : "枯树盘根",
	"damage_type" : "震伤"
]),
});

int valid_enable(string usage) { return usage == "axe" || usage == "parry"; }

int valid_learn(object me)
{
	object weapon;
	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "axe")
		return notify_fail("你使用的武器不对。\n");
	if ((int)me->query("max_neili") < 200)
		return notify_fail("你的内力不够。\n");
	if (me->query_str(1) < 25)
		return notify_fail("你的臂力太低。\n");
	if ((int)me->query_skill("qiantian-yiyang", 1) < 15
	&& (int)me->query_skill("kurong-changong", 1) < 15)
		return notify_fail("你的内功火候太浅。\n");
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
	int i, j, level;
	level = (int) me->query_skill("pangen-fu",1);

	for(i = sizeof(action); i > 0; i--){
		if(level > action[i-1]["lvl"]){
			j = NewRandom(i, 20, level/4);
			return ([  
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"force":250+random(250),
				"damage":150+random(120),
				"skill_class"   : GANG,
				"skill_damage"  : 50,
				"skill_attack"  : -10,
				"skill_parry"   : 20,
				"skill_dodge"   : -20,
				"skill_kill"    : -100,
				"skill_rush"    : -100,
				"skill_hurt"    : 100,
				"skill_GP"      : 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("pangen-fu", 1);
	int i = sizeof(action);
	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 30)
		return notify_fail("你的精力不够练「盘根错结斧」。\n");
	if((int)me->query("neili") < 10)
		return notify_fail("你的内力不够练「盘根错结斧」。\n");
	me->receive_damage("jingli", 20);
	me->add("neili", -5);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"tls/pangen-fu/" + action;
}
