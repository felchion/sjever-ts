// dashou-yin.c 密宗大手印
//  by iceland

inherit SKILL;
#include <skill_class.h>

string query_class() {return "奇";}

mapping *action = ({
([
	"action" : "$N使出一招「莲花合掌印」，双掌合十，直直撞向$n的前胸",
	"skill_name" : "莲花合掌印",
	"lvl" : 0,
	"damage_type" : "瘀伤",
]),
([
	"action" : "$N使出一招「合掌观音印」，飞身跃起，双手如勾，抓向$n的$l",
	"skill_name" : "合掌观音印",
	"lvl" : 10,
	"damage_type" : "瘀伤",
]),
([
	"action" : "$N使出一招「准提佛母印」，运力于指，直取$n的$l",
	"skill_name" : "准提佛母印",
	"lvl" : 20,
	"damage_type" : "瘀伤",
]),
([
	"action" : "$N使出一招「红阎婆罗印」，怒吼一声，一掌当头拍向$n的$l",
	"skill_name" : "红阎婆罗印",
	"lvl" : 30,
	"damage_type" : "瘀伤",
]),
([
	"action" : "$N使出一招「药师佛根本印」，猛冲向前，掌如游龙般攻向$n",
	"skill_name" : "药师佛根本印",
	"lvl" : 40,
	"damage_type" : "瘀伤",
]),
([
	"action" : "$N使出一招「威德金刚印」，伏身疾进，双掌自下扫向$n的$l",
	"skill_name" : "威德金刚印",
	"lvl" : 50,
	"damage_type" : "瘀伤",
]),
([
	"action" : "$N使出一招「上乐金刚印」，飞身横跃，双掌前后击出，抓向$n的咽喉",
	"skill_name" : "上乐金刚印",
	"lvl" : 60,
	"damage_type" : "瘀伤",
]),
([
	"action" : "$N使出一招「六臂智慧印」，顿时劲气弥漫，天空中出现无数掌影打向$n的$l",
	"skill_name" : "六臂智慧印",
	"lvl" : 70,
	"damage_type" : "瘀伤",
]),
});

int valid_enable(string usage) { return usage == "hand" || usage == "parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练密宗大手印必须空手。\n");
	if ( me->query("max_neili") < 100)
		return notify_fail("你的内力太弱了，练不了密宗大手印。\n");
	if ( me->query_skill("longxiang-boruo", 1) < 20)
		return notify_fail("你的龙象般若功火候太浅，不能学密宗大手印。\n");
	if ( me->query_skill("force", 1) < 20)
		return notify_fail("你的基本内功火候太浅，不能学密宗大手印。\n");
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
	level   = (int) me->query_skill("dashou-yin",1);
	for(i = sizeof(action); i > 0; i--)
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([  
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"skill_class" 	: QI,
				"skill_damage" 	: 10 ,
				"skill_attack" 	: 20 ,
				"skill_parry" 	: 10 ,
				"skill_dodge"		: 10 ,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : 0,
				"skill_GP" 			: 2
			]);
		}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("dashou-yin", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 30)
		return notify_fail("你的体力不够，练不了密宗大手印。\n");
	if ( me->query("neili") < 10)
		return notify_fail("你的内力不够，练不了密宗大手印。\n");
	me->receive_damage("jingli", 20);
	me->add("neili", -5);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"dls/dashou-yin/" + action;
}
