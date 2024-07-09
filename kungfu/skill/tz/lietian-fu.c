// lietian-fu.c 裂天斧法

inherit SKILL;
#include <ansi.h>
#include <skill_class.h>
string query_class() {return "险";}

mapping *action = ({
([     "action" : "$N平举手中$w，一招「风云起」，欺身向前，向$n的$l撩去",
       "lvl" : 0,
       "skill_name" : "风云起",
       "damage_type" : "割伤"
]),
([     "action" : "$N一招「力推山」，马步一站,手推$w背，$w平平向$n的颈部抹去",
       "lvl" : 30,
       "skill_name" : "力推山",
       "damage_type" : "割伤"
]),
([     "action" : "$N掀起漫天$w影，狂冲乱打,一招「狂风破」，罡风笼罩$n的全身",
       "lvl" : 40,
       "skill_name" : "狂风破",
       "damage_type" : "内伤"
]),
([     "action" : "$N一招「力开山」，招式重拙,大开大阖，高举头顶,直劈向$n",
       "lvl" : 60,
       "skill_name" : "日月晦明",
       "damage_type" : "劈伤"
]),
([     "action" : "$N手中$w一沉，气凝斧上,一招「震天地」，双手挥起$w砍向$n的胸口",
       "lvl" : 90,
       "skill_name" : "震天地",
       "damage_type" : "震伤"
]),
});


int valid_enable(string usage) { return usage == "axe" || usage == "parry"; }

int valid_learn(object me)
{
       if ((int)me->query("max_neili") < 300)
               return notify_fail("你的内力不够。\n");
       if (me->query_str() < 30)
               return notify_fail("你的臂力太低。\n");
       if ((int)me->query_skill("guiyuan-tunafa", 1) < 20)
               return notify_fail("你的「归元吐纳法」火候太浅。\n");
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
	object target;
	level   = (int) me->query_skill("lietian-fu",1);
	for(i = sizeof(action); i > 0; i--){
		if(level > action[i-1]["lvl"]){
			j = NewRandom(i, 20, level/4);
			return ([  
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"skill_class"   : XIAN,
				"skill_damage"  : 20,
				"skill_attack"  : -10,
				"skill_parry"   : -50,
				"skill_dodge"   : 20,
				"skill_kill"    : 30,
				"skill_rush"    : 50,
				"skill_hurt"    : 0,
				"skill_GP"      : 2
			]);
		}
	}
}

int practice_skill(object me)
{
       object weapon;
       int lvl = me->query_skill("lietian-fu", 1);
       int i = sizeof(action);
        while (i--) if (lvl == action[i]["lvl"]) return 0;

       if (!objectp(weapon = me->query_temp("weapon"))
       || (string)weapon->query("skill_type") != "axe")
               return notify_fail("你使用的武器不对。\n");
       if ((int)me->query("qi") < 50)
               return notify_fail("你的体力不够练「裂天斧法」。\n");
       me->receive_damage("qi", 25);
       return 1;
}

string perform_action_file(string action)
{
       return __DIR__"tz/lietian-fu/" + action;
}
