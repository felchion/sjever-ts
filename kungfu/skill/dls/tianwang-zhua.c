// tianwang-zhua.c 金翅天王爪
//  by iceland
inherit SKILL;
#include <skill_class.h>

string query_class() {return "险";}

mapping *action = ({
([
	"action" : "$N含胸探身，一式「凝云」，左手在胸前成爪虚护，右爪直探$n的面门",
	"lvl" : 0,
	"skills_name" : "凝云",
	"damage_type" : "抓伤"
]),
([
	"action" : "$N一式「捻灯」，弓腰区背，足底发劲，斜身抢到$n身侧，单手扣向$n脖项",
	"lvl" : 10,
	"skills_name" : "捻灯",
	"damage_type" : "抓伤"
]),
([
	"action" : "$N喋喋怪啸，摄人心魄，一式「妙音」，期进$n怀中，双爪环扣，抱向$n",
	"lvl" : 20,
	"skills_name" : "妙音",
	"damage_type" : "震伤"
]),
([
	"action" : "$N腾空跃起，使一式「清乐」，双爪前后交错出击，一抓快似一抓，逼得$n连连倒退",
	"lvl" : 30,
	"skills_name" : "清乐",
	"damage_type" : "抓伤"
]),
([
	"action" : "$N双目半睁半闭，口中念念有词，一式「撞钟」，左手搭在右手之上，右爪合双手之力直催向$n的$l",
	"lvl" : 40,
	"skills_name" : "撞钟",
	"damage_type" : "震伤"
]),
([
	"action" : "$N端坐不动，一式「暗香」，双爪在身前虚按，幻出无数爪影，指端真气破空丝丝做响，遥遥罩向$n全身",
	"lvl" : 50,
	"skills_name" : "暗香",
	"damage_type" : "抓伤"
])
});

int valid_enable(string usage) { return usage=="claw" || usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练金翅天王爪必须空手。\n");
	if ( me->query_skill("longxiang-boruo", 1) < 30)
		return notify_fail("你的龙象般若功火候不够，不能学金翅天王爪。\n");
	if ( me->query_skill("force", 1) < 30)
		return notify_fail("你的基本内功火候不够，不能学金翅天王爪。\n");
	if ( me->query("max_neili") < 150)
		return notify_fail("你的内力太弱，练不了金翅天王爪。\n");
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
	level   = (int) me->query_skill("tianwang-zhua",1);
	for(i = sizeof(action); i > 0; i--)
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([  
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"weapon": action[j]["weapon"],
				"skill_class" 	: XIAN,
				"skill_damage" 	: 10 ,
				"skill_attack" 	: 20 ,
				"skill_parry" 	: -20 ,
				"skill_dodge"		: 0 ,
				"skill_kill" 		: 0,
				"skill_rush" 		: 30,
				"skill_hurt" 	  : 50,
				"skill_GP" 			: 2
			]);
		}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("tianwang-zhua", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;
	if ( me->query("jingli") < 30)
		return notify_fail("你的体力太低了。\n");
	if ( me->query("neili") < 10)
		return notify_fail("你的内力不够练金翅天王爪。\n");
	me->receive_damage("jingli", 20);
	me->add("neili", -5);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"dls/tianwang-zhua/" + action;
}
