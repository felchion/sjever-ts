// canhe-zhi.c 参合指
// by leontt 1/5/2000

#include <ansi.h>
#include <skill_class.h>
inherit SKILL;

string query_class() {return "妙";}

mapping *action = ({
([
	"action" : "$N劲风激荡，双指错落搭置，一招「目不识丁」凶狠地向$n的$l插去",
	"lvl" : 0,
	"skill_name" : "目不识丁",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N大吼一声，十指上下分飞，一招「画龙点睛」，双指直取$n的$l",
	"lvl" : 20,
	"skill_name" : "画龙点睛",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N十指互错，虚幻虚实，一招「扑朔迷离」，前后左右，瞬息间向$n攻出了六招",
	"lvl" : 40,
	"skill_name" : "扑朔迷离",
	"damage_type" : "刺伤",
]),
([
	"action" : "$N身行腾空而起，一招「受宠若惊」，来势奇快，向$n的$l猛插下去",
	"lvl" : 60,
	"skill_name" : "受宠若惊",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N身行一闪，若有若无捱到$n身前，一招「滔滔不绝」，插向$n的$l",
	"lvl" : 70,
	"skill_name" : "滔滔不绝",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N一招「退避三舍」，右手一拳击出，左掌化指，一掌一指凌空击向$n的$l",
	"lvl" : 80,
	"skill_name" : "退避三舍",
	"damage_type" : "内伤"
]),
([
	"action" : "$N双掌化指，指中带掌，双手齐推，一招「卧薪尝胆」，一股排山倒海的内力，直扑$n$l",
	"lvl" : 90,
	"skill_name" : "卧薪尝胆",
	"damage_type" : "内伤"
]),
([
	"action" : "$N突然身行急转，一招「相煎何急」，十指飞舞，霎时之间将$n四面八方都裹住了",
	"lvl" : 100,
	"skill_name" : "相煎何急",
	"damage_type" : "震伤"
]),
});

int valid_enable(string usage) { return usage=="finger" || usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练参合指必须空手。\n");
	if ( me->query_skill("shenyuan-gong", 1) < 15)
		return notify_fail("你的神元功火候不够，无法学参合指。\n");
	if ( me->query("max_neili") < 100)
		return notify_fail("你的内力太弱，无法练参合指。\n");
	return 1;
}

string query_skill_name(int level)
{
	int i = sizeof(action);
	while (i--)
		if(level >= action[i]["lvl"])
			return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
	int i, level, j;
	level   = (int) me->query_skill("canhe-zhi",1);

	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
				"force": 200 + random(350),
				"skill_class" 	: MIAO,
			  "skill_damage" 	: 0,
			  "skill_attack" 	: 20,
			  "skill_parry" 	: 0,
			  "skill_dodge"		: 30,
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
	int lvl = me->query_skill("canhe-zhi", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 50)
		return notify_fail("你的精力太低了。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练参合指。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{        
	return __DIR__"mr/canhe-zhi/" + action;
}

mixed hit_ob(object me, object target)
{
	string msg;
	int ap,dp;
 
	if ( me->query_temp("mr_ci") ) return 0;
  
  ap = me->query_int()+me->query("int");
	dp = target->query_int()+target->query("int");
	
	if ( random(ap) > dp/3 && !random(4) ) {
		switch(random(2)) {
			case 0 :
				msg = HIW"$N默运神元神功，指风轻飘飘地点向$n，一股内劲无声无息的绕向$n周身大穴！\n";
				msg += "$n只觉全身一麻，已被一招点中「神阖穴」！\n"NOR; 
				target->add_busy(2+random(2));
				break;
			case 1 :
				msg = HIR"$N指风犀利，似一道无形剑气，将$n笼罩于内！$n只觉胸口一痛，全身真气狂泻而出。\n"NOR;
				target->recevied_damage("neili", me->query_skill("canhe-zhi",1)*5);
				if ( target->query("neili") < 0 )
					target->set("neili", 0);
				break;
		}
		message_vision(msg, me, target);
	}
}
