// longzhua-gong.c -龙爪功

#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

string query_class() {return "险";} 


mapping *action = ({
([
	"action" : "只听“哧”的一声，急风响过，$N使一招「捕风式」，右手猛然探出，去势犹如闪电",
	"lvl"   : 0,
	"skill_name" : "捕风式" ,               
	"damage_type" : "抓伤"
]),
([
	"action" : "$N凌空掠起，动作有如长虹惊天，一招「捉影式」，如矢应机，霆不暇发",
	"lvl"   : 10,
	"skill_name" : "捉影式" ,               
	"damage_type" : "抓伤"
]),
([
	"action" : "$N手腕一翻，使出「抚琴式」，十指微微上下抖动，气流相交的“呲呲”声登时有如繁弦急管一般",
	"lvl"   : 20,
	"skill_name" : "抚琴式" ,               
	"damage_type" : "抓伤"
]),
([
	"action" : "$N催动内力，双爪互错，使出「鼓瑟式」，招招连绵不绝，犹似行云流水一般",
	"lvl"   : 40,
	"skill_name" : "鼓瑟式" ,               
	"damage_type" : "抓伤"
]),
([
	"action" : "$N使一招「批亢式」，招数纵横开阖，内力浑雄，猛地击向$n的$l",
	"lvl"   : 60,
	"skill_name" : "批亢式" ,               
	"damage_type" : "抓伤"
]),
([
	"action" : "$N凌空翻身，右手带着凌厉风声，一招「掏虚式」击向$n的$l",
	"lvl"   : 80,
	"skill_name" : "掏虚式" ,               
	"damage_type" : "抓伤"
]),
([ 
	"action" : "$N招招进逼，一招「抢珠式」，直取$n$l，每一爪都有石破天惊，风雨大至之势",
	"lvl"   : 100,
	"skill_name" : "抢珠式" ,               
	"damage_type" : "抓伤"
]),
([
	"action" : "$N身形流转，飕飕飕连环三爪，一招「捞月式」，破空之声甚是凌厉",
	"lvl"   : 120,
	"skill_name" : "捞月式" ,               
	"damage_type" : "抓伤"
]),
([
	"action" : "$N骨节“咯咯咯”一阵暴响，手臂徒长半尺，一招「抱残式」，五指猛然叉向$n胸前大穴",
	"lvl"   : 150,
	"skill_name" : "抱残式" ,               
	"damage_type" : "抓伤"
]),
([ 
	"action" : "$N猛地一声低嗥，爪法忽变，一招「守缺式」出招随缓，但力道却凝重强劲",
	"lvl"   : 180,
	"skill_name" : "守缺式" ,               
	"damage_type" : "抓伤"
])
});

int valid_enable(string usage) { return usage=="claw" ||  usage=="parry"; }

int valid_combine(string combo) 
{
	object me;
	me = this_player(); 

	if ( me->query_int(1) > 40)
		return combo=="yizhi-chan"; 
}

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练龙爪功必须空手。\n");
	if ((int)me->query_skill("yijin-jing", 1) < 60)
		return notify_fail("你的易筋经内功火候不够，无法学龙爪功。\n");
	if ((int)me->query_int(1) > 30) {
		return 1;
	}
	else {
		if ( me->query_skill("banruo-zhang", 1)
		 && me->query_skill("yizhi-chan", 1)
		 && me->query_skill("jingang-quan", 1)
		 && me->query_skill("nianhua-zhi", 1)
		 && me->query_skill("qianye-shou", 1)
		 && me->query_skill("ruying-suixingtui", 1)
		 && me->query_skill("sanhua-zhang", 1))
			return notify_fail("你的功力不够，无法同时学习两种以上的上乘功夫。\n");
	}
	if ((int)me->query("max_neili") < 600)
		return notify_fail("你的内力太弱，无法练龙爪功。\n");
	return 1;
}

string query_skill_name(int level)
{
	int i;
	for(i = sizeof(action); i > 0; i--)
		if(level >= action[i-1]["lvl"])
			return action[i-1]["skill_name"];
}

mapping query_action(object me, object weapon)
{
	int i, j, level;
	
	level = (int) me->query_skill("longzhua-gong",1);
	for(i = sizeof(action); i > 0; i--) 	{
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"damage_type": action[j]["damage_type"],
				"lvl": action[j]["lvl"],
				"skill_class" 	: XIAN,
				"skill_damage" 	: 30 ,
				"skill_attack" 	: 20 ,
				"skill_parry" 	: -20 ,
				"skill_dodge"		: -10 ,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : 30,
				"skill_GP" 			: 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("longzhua-gong", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 40)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练龙爪功。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"sl/longzhua-gong/" + action;
}

