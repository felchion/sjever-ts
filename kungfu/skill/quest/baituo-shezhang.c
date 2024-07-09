// baituo-shezhang.c 白陀蛇杖

#include <ansi.h>
inherit SKILL;

mapping *action = ({
([
	"action": "$N高高跃起，挥舞着手中的$w一招「一柱擎天」犹如一条黑蟒般向$n当头直落而下",
	"lvl" : 0,
	"damage_type": "砸伤"
]),
([
	"action": "$N一声暴喝「横扫千军」！将$w由下往上一撩，双手握住$w尾，转身猛得横扫打向$n的$l",
	"lvl" : 10,
	"damage_type": "割伤"
]),
([
	"action": "$N忽然招数一变，使出「灵蛇出洞」，杖法显得灵巧之极，手中$w化作条条蛇影缠向$n",
	"lvl" : 20,
	"damage_type": "砸伤"
]),
([
	"action": "$N以杖代剑，$w中宫直进，夹着一阵狂风刺出，逼向$n的$l，正是招「蛟蛇化龙」",
	"lvl" : 40,
	"damage_type": "刺伤"
]),
([
	"action": "$N一式「一杖定海」，当头一$w击将下来，杖头未至，一股风已将$n逼得难以喘气",
	"lvl" : 60,
	"damage_type": "砸伤"
]),
([
	"action": "$N变招「群蛇狂舞」，$w扫出一道道灰影从四面八方围向$n，要将$n淹没吞食",
	"lvl" : 80,
	"damage_type": "瘀伤"
]),
});

int valid_enable(string usage) { return usage=="staff" || usage=="parry"; }

string query_skill_name(int level)
{
	int i = sizeof(action);
	while (i--)
		if(level > action[i]["lvl"])
			return action[i]["skill_name"];
}

mapping query_action(object me)
{
	object target, weapon; 
	int i, level, lvl, j;

	target = offensive_target(me);
	if (target->query_temp("weapon"))
		weapon = target->query_temp("weapon");
    
	lvl = random((int)me->query_skill("baituo-shezhang",1));
      
	if (me->query_skill("baituo-shezhang",1) > 100
	&& me->query("neili") > 1000 
	&& random(5) > 3 
	&& random(me->query("combat_exp")) > target->query("combat_exp")/2){
		me->add("neili", -20);            
		return ([
			"action":	HIC"不等招式用老，$N把手中$w顺势一晃，杖尖直划而下，夹起一股阴风劈向$n！"NOR,
			"force":	400+random(250),
			"dodge":	30,
			"parry":	30,
			"damage":	180 +random(100),
			"damage_type":	"割伤"
		]);
	}
	if (objectp(weapon) 
	&& weapon->query("skill_type") == "stick" 
	&& target->query_skill_mapped("stick") == "dagou-bang"
	&& lvl > target->query_skill("bagou-bang",1)/2 
	&& me->query("neili") > 100 
	&& lvl >= 140 
	&& me->query("oyf_hamagong") > 2) {                                                    
		me->add("neili", -100);
		return ([
			"action":	HIC"$N右手握杖直挺，凝聚功力于杖上，身行随风飘摆，而杖尖任他狂风暴雨也不移动，\n"+
					"单单直指$n心口，正是打狗棒第三十六路棒法「天下无狗」的第六变的棒心所在！"NOR,
			"force" :	600,
			"dodge" :	random(30),
			"parry":	random(35),
			"damage":	180 +random(150),
			"damage_type":	"刺伤",
		]);
	}
	level   = (int) me->query_skill("baituo-shezhang",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action":	action[j]["action"],
				"lvl":		action[j]["lvl"],
				"damage_type":	action[j]["damage_type"],
				"parry":	random(30),
				"dodge":	random(30),
				"force":	300 + random(350),
				"damage":	100 + random(180),
			]);
		}
	}
}

int practice_skill(object me)
{
	if ((int)me->query("jingli") < 60)
		return notify_fail("你的体力不够了。\n");
	if ((int)me->query("neili") < 30)
		return notify_fail("你的体力不够了。\n");
	me->receive_damage("jingli", 50);
	me->add("neili", -20);
	return 1;
}

int valid_learn(object me)
{
	object weapon;

	if( (int)me->query("max_neili") < 500 )
		return notify_fail("你的内力不足，没有办法练白陀蛇杖, 多练些内力再来吧。\n");

	if (!me->query_skill("hamagong", 1) || me->query_skill("hamagong", 1) < 50)
		return notify_fail("你的蛤蟆功火候太浅。\n");

	if ( !objectp(weapon = me->query_temp("weapon"))
	|| ( string)weapon->query("skill_type") != "staff" )
		return notify_fail("你必须使用杖条来练杖法。\n");
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"quest/baituo-shezhang/" + action;
}
