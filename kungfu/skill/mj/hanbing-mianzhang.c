// hanbing-mianzhang.c 寒冰绵掌

inherit SKILL;
#include <skill_class.h>

string query_class() {return "慢";}

mapping *action = ({
([
	"action" : "$N双掌疾伦，一式「如罩寒霜」，又臂突然划起一道半孤，罩向$n的全身",
	"damage_type" : "瘀伤",
	"lvl" : 0,
	"skill_name" : "如寒冰头"
]),
([
	"action" : "$N双腿合并，呼的向上纵起，一式「如寒冰头」，二掌直下，攻向$n的头部",
	"damage_type" : "瘀伤",
	"lvl" : 0,
	"skill_name" : "如寒冰头"
]),
([
	"action" : "$N左掌护胸，一式「白露为霜」，右掌直线击出，疾拍$n的胸前大穴",
	"damage_type" : "瘀伤",
	"lvl" : 10,
	"skill_name" : "白露为霜"
]),
([
	"action" : "$N使一式「寒梅纷飞」，双掌绵绵不绝的拍出，似朵朵雪中红梅罩向$n",
	"damage_type" : "劈伤",
	"lvl" : 20,
	"skill_name" : "寒梅纷飞"
]),
([
	"action" : "$N深吸一口气，右拳凝劲后发，一式「天寒地冻」，一股寒流缓缓推向$n的$l",
	"damage_type" : "瘀伤",
	"lvl" : 30,
	"skill_name" : "天寒地冻"
]),
([
	"action" : "$N使一式「冰天雪地」，双掌突的变成雪白，一前一后，猛地拍向$n的$l",
	"damage_type" : "瘀伤",
	"lvl" : 40,
	"skill_name" : "冰天雪地"
]),
([
	"action" : "$N双目微张，一式「雪花纷飞」，双掌化作片片雪花疾拍向$n的$l",
	"damage_type" : "内伤",
	"lvl" : 50,
	"skill_name" : "雪花纷飞"
]),
([
	"action" : "$N向上高高跃起，一式「满天冰雨」，居高临下，掌力似冰似雨笼罩$n的全身",
	"damage_type" : "内伤",
	"lvl" : 70,
	"skill_name" : "满天冰雨"
])
});

int valid_enable(string usage) { return usage=="strike" ||  usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练寒冰绵掌必须空手。\n");
	if(!me->query_skill("jiuyang-shengong", 1))
	if ((int)me->query_skill("shenghuo-shengong", 1) < 20)
		return notify_fail("你的圣火神功火候不够，无法学寒冰绵掌。\n");
	if ((int)me->query("max_neili") < 100)
		return notify_fail("你的内力太弱，无法练寒冰绵掌。\n");
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
	level = (int)me->query_skill("hanbing-mianzhang", 1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
				"skill_class" 	: MAN,
				"skill_damage" 	: 20 ,
				"skill_attack" 	: 0 ,
				"skill_parry" 	: 30 ,
				"skill_dodge"		: -30 ,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : 100,
				"skill_GP" 			: 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("hanbing-mianzhang", 1);
	int i = sizeof(action);
	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 30)
		return notify_fail("你的精力太低了。\n");
	if ((int)me->query("neili") < 10)
		return notify_fail("你的内力不够练寒冰绵掌。\n");
	me->receive_damage("jingli", 20);
	me->add("neili", -5);
	return 1;
}

mixed hit_ob(object me, object victim, int damage_bonus, int factor)
{
	if( random(me->query("neili")) > victim->query("neili")/2 
	 && random(me->query_skill("hanbing-mianzhang", 1)) >= 80
	 && !victim->query("job_npc")
	 && random(2) 
	 && !userp(victim))
		victim->add_condition("hb_poison", random(6)+ 2);
}

string perform_action_file(string action)
{
	return __DIR__"mj/hanbing-mianzhang/" + action;
}
