// tianshan-zhang.c, 天山杖法
// by pishou
// modified 内敛毒性 by caiji@SJ 5/17/2001
// Modified by lklv for killer

#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

//string query_class() {return "妙";}

mapping *action = ({
([
	"action": "$N高举$w, 一招「雪海冰雹」，从天而降砸向$n的$l",
	"lvl" : 0,
	"damage_type": "砸伤"
]),
([
	"action": "$N使出一招「天山雪野」，手中$w封住下盘扫向$n的$l",
	"lvl" : 30,
	"damage_type": "砸伤"
]),
([
	"action": "$N使出一招「满天飞雪」，挥动手中$w从各个方向点往$n的$l",
	"lvl" : 60,
	"damage_type": "刺伤"
]),
([
	"action": "忽听得呼一声猛响，$N一招「雪峰插云」，手握$w飞身砸向$n的$l",
	"lvl" : 90,
	"damage_type": "砸伤"
]),
});

string query_skill_name(int level)
{
	int i = sizeof(action);
	while (i--)
		if(level > action[i]["lvl"])
			return action[i]["skill_name"];
}

int valid_enable(string usage) { return (usage == "staff") || (usage == "parry"); }

int valid_learn(object me)
{
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon"))
	 || (string)weapon->query("skill_type") != "staff")
		return notify_fail("你手上的武器不能用来练天山杖法。\n");

	if ((int)me->query("max_neili") < 200)
		return notify_fail("你还是先集中精力修练内力吧。\n");
	return 1;
}

int practice_skill(object me)
{
	int lvl = me->query_skill("tianshan-zhang", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 40)
		return notify_fail("你先歇口气再练吧。\n");

	if ( me->query("neili") < 20)
		return notify_fail("你先歇口气再练吧。\n");

	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

mapping query_action(object me, object weapon)
{
	int i, j,level;
  
  level = (int)me->query_skill("tianshan-zhang", 1);
	for(i = sizeof(action); i > 0; i--)
		if(level >= action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action" : action[j]["action"],
				"lvl" : action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
	//			"skill_class" 	: MIAO,
				"skill_damage" 	: 10 ,
				"skill_attack" 	: 30 ,
				"skill_parry" 	: 20 ,
				"skill_dodge"		: -10 ,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : 0,
				"skill_GP" 			: 2
			]);
		}
}

mixed hit_ob(object me, object victim, int damage_bonus, int factor)
{
	int lvl_temp = me->query_skill("tianshan-zhang", 1);

	if (lvl_temp >=140
	&& me->query("env/化功大法") != "内敛"
	&& me->query_skill("poison", 1) > 80
	&& (me->query("family/family_name") == "星宿派"	|| me->query("combat_exp", 1) > 2000000 )
	&& random(me->query_skill("poison", 1)) > random( victim->query_skill("medicine", 1))
	&& !victim->query("job_npc")
	&& !userp(victim)) {
		if( random(lvl_temp) <= 100) {
			victim->add_condition("xx_poison", random( me->query_skill("poison",1)/30) + 5);
			tell_object(victim, HIG "你觉得眼前一阵发晕！似乎中了毒，原来对方杖上抹的毒药正在散发着毒气！\n" NOR );
		}
		else if( random(lvl_temp) > 100) {
			victim->add_condition("xx_poison", random( me->query_skill("poison",1)/15) + 5 );
			tell_object(victim, HIG "你觉得被击中的地方一阵麻木！似乎中了毒，原来对方的杖上涂抹有毒药！\n" NOR );
		}
	}
}

string perform_action_file(string action)//add by caiji
{
	return __DIR__"xx/tianshan-zhang/" + action;
}
