// 增加 pfm 叠掌特殊效果 by Lane@SJ
// kunlun-zhang.c 昆仑掌

#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

string query_class() {return "慢";}  

mapping *action = ({
([
	"action" : "$N掌风激荡，双掌错落，来势神妙无方，一招「日入空山」，劈向$n的$l",
	"lvl" : 0,
	"skill_name" : "日入空山",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N双掌骤起，一招「天清云淡」，一掌击向$n面门，另一掌却按向$n小腹",
	"lvl" : 30,
	"skill_name" : "天清云淡",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N双掌互错，变幻莫测，一招「秋风不尽」，前后左右，瞬息之间向$n攻出了四四一十六招",
	"lvl" : 60,
	"skill_name" : "秋风不尽",
	"damage_type" : "瘀伤",
]),
([
	"action" : "$N一声清啸，呼的一掌，一招「山回路转」，自巧转拙，却是去势奇快，向$n的$l猛击过去",
	"lvl" : 100,
	"skill_name" : "山回路转",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N双掌交错，若有若无，一招「天衣无缝」，拍向$n的$l",
	"lvl" : 120,
	"skill_name" : "天衣无缝",
        "damage_type" : "瘀伤"
]),
([
	"action" : "$N一招「青山断河」，右手一拳击出，左掌紧跟着在右拳上一搭，变成双掌下劈，击向$n的$l",
	"lvl" : 140,
	"skill_name" : "青山断河",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N双手齐划，跟着双掌齐推，一招「北风卷地」，一股排山倒海的掌力，直扑$n面门",
	"lvl" : 160,
	"skill_name" : "北风卷地",
	"damage_type" : "内伤"
]),
([
	"action" : "$N突然滴溜溜的转身，一招「天山雪飘」，掌影飞舞，霎时之间将$n四面八方都裹住了",
	"lvl" : 180,
	"skill_name" : "天山雪飘",
	"damage_type" : "瘀伤"
])
});

int valid_enable(string usage) { return usage=="strike" ||  usage=="parry"; }

int valid_learn(object me)
{
	mapping fam  = me->query("family");
	if (fam["family_name"] != "昆仑派" || fam["generation"] != 2)
		return notify_fail("昆仑掌只能从何足道处学到。\n");
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练昆仑掌必须空手。\n");
	if ( me->query_skill("xuantian-wuji", 1) < 120)
		return notify_fail("你的玄天无极功火候不够，无法练昆仑掌。\n");
	if ( me->query("max_neili") < 800)
		return notify_fail("你的内力修为不够，无法练昆仑掌。\n");
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
	level = (int) me->query_skill("kunlun-zhang",1);

	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
	      "skill_class"   : MAN,
	      "skill_damage"  : 20,
	      "skill_attack"  : 0,
	      "skill_parry"   : 30,
	      "skill_dodge"   : 0,
	      "skill_kill"    : 0,
	      "skill_rush"    : 0,
	      "skill_hurt"    : 50,
	      "skill_improve" : 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("kunlun-zhang", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 50)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练昆仑掌。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"kl/kunlun-zhang/" + action;
}

