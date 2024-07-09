//什么事也没有，打倒BBB
// yizhi-chan.c 一指禅

#include <ansi.h>
inherit SKILL;

#include <skill_class.h>

string query_class() {return "正";} 

mapping *action = ({
([
	"action" : "$N双指并拢，一式「佛恩济世」，和身而上，左右手一前一后戳向$n的胸腹间",
	"lvl" : 0,
	"skill_name" : "佛恩济世",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N左掌护胸，一式「佛光普照」，右手中指前后划了个半弧，猛地一甩，疾点$n的$l",
	"lvl" : 20,
	"skill_name" : "佛光普照",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N身形闪动，一式「佛门广渡」，双手食指端部各射出一道青气，射向$n的全身要穴",
	"lvl" : 40,
	"skill_name" : "佛门广渡",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N盘膝跌坐，一式「佛法无边」，左手握拳托肘，右手拇指直立，遥遥对着$n一捺",
	"lvl" : 60,
	"skill_name" : "佛法无边",
	"damage_type" : "刺伤"
])
});

int valid_enable(string usage) { return usage=="finger" || usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练一指禅必须空手。\n");
	if ((int)me->query_skill("yijin-jing", 1) < 60)
		return notify_fail("你的易筋经内功火候不够，无法学一指禅。\n");
	if ((int)me->query_int(1)> 30) {
		return 1;
	}
	else {
		if ( me->query_skill("banruo-zhang", 1)
		 && me->query_skill("longzhua-gong", 1)
		 && me->query_skill("jingang-quan", 1)
		 && me->query_skill("nianhua-zhi", 1)
		 && me->query_skill("qianye-shou", 1)
		 && me->query_skill("ruying-suixingtui", 1)
		 && me->query_skill("sanhua-zhang", 1))
			return notify_fail("你的功力不够，无法同时学习两种以上的上乘功夫。\n");
	}
	if ((int)me->query("max_neili") < 600)
		return notify_fail("你的内力太弱，无法练一指禅。\n");
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
	level   = (int) me->query_skill("yizhi-chan",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"damage_type": action[j]["damage_type"],
				"lvl": action[j]["lvl"],
				"skill_class" 	: ZHENG,
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
}

int practice_skill(object me)
{
	int lvl = me->query_skill("yizhi-chan", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;
	if ((int)me->query("jingli") < 40)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练一指禅。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

mixed hit_ob(object me, object target)
{
	string msg;
	int j;
	j = me->query_skill("yizhi-chan", 1);
   
	if( random(20) >= 15 
	&& !target->is_busy() 
	&& j > 200 
	&& me->query("neili") > target->query("neili")
	&& me->query("neili") > 1000 
	&& me->query("max_neili") > 1500 
	&& random(j) > random(target->query_skill("dodge",1))){
		switch(random(2)){
			case 0 :
				msg = HIY"$N身形闪动，一式「佛门广渡」，双手食指端部各射出一道青气，射向$n的全身要穴！\n";
				msg += "$n只觉全身一麻，已被这一招「佛门广渡」点中！\n"NOR; 
				target->start_busy(2+random(2));
				break;
			case 1 :
				msg = HIW"$N双指并拢，一式「佛恩济世」，和身而上，左右手一前一后戳向$n的胸腹间！\n";
				msg += "结果$n气血一滞，已被定在当堂。\n"NOR;
				target->start_busy(2+random(2));
				break;
		}
		message_vision(msg, me, target);
	}
}

string perform_action_file(string action)
{
	return __DIR__"sl/yizhi-chan/" + action;
}
