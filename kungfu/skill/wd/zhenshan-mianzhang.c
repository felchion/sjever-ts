// zhenshan-mianzhang.c 震山绵掌
#include <ansi.h>
#include <skill_class.h>
inherit SKILL;


mapping *action = ({
([
	"action" : "$N低喝一声：“好！”一个「虎纵步」，闪开正面，踏上一步，右掌一招「划手」，向$n的$l击去",
	"damage_type" : "瘀伤",
	"lvl" : 0,
	"skill_name" : "划手"
]),
([
	"action" : "$N身形一矮，一个「印掌」，震山掌掌风飒然如斧，劈向$n的$l",
	"damage_type" : "瘀伤",
	"lvl" : 10,
	"skill_name" : "印掌"
]),
([
	"action" : "$N纵身上前，左手一掌「拨云见日」，一股排山倒海的掌力按向$n的$l",
	"damage_type" : "瘀伤",
	"lvl" : 15,
	"skill_name" : "拨云见日"
]),
([
	"action" : "$N双掌一错，凝式不发，突然变招左腿扫向$n的$l，正是一招「横云断峰」",
	"damage_type" : "瘀伤",
	"lvl" : 20,
	"skill_name" : "横云断峰"
]),
([
	"action" : "$N变掌为爪，抓向$n的$l，一招「倒提金钟」，欲把$n身躯提起摔出",
	"damage_type" : "瘀伤",
	"lvl" : 25,
	"skill_name" : "倒提金钟"
]),
([
	"action" : "$N使出「春风拂柳」，在$n脸前虚势一扬，接着化掌为指，伸食中双指从下而上戳向$n手肘「曲池穴」",
	"damage_type" : "内伤",
	"lvl" : 30,
	"skill_name" : "春风拂柳"
]),
([
	"action" : "$N运力右掌，一招「烘云托月」，手腕翻过，下肘转了个小圈，向$n的$l打去",
	"damage_type" : "瘀伤",
	"lvl" : 35,
	"skill_name" : "烘云托月"
]),
([
	"action" : "$N收转手掌，变招握拳，一式「雪拥蓝关」，向$n的$l猛击",
	"damage_type" : "瘀伤",
	"lvl" : 40,
	"skill_name" : "雪拥蓝关"
]),
([
	"action" : "$N下盘「盘弓射雕」，双手反转，以擒拿手法反扣$n的$l的要穴脉门",
	"damage_type" : "瘀伤",
	"lvl" : 45,
	"skill_name" : "盘弓射雕"
]),
([
	"action" : "$N发招「鸳鸯腿」攻向$n的$l，随即点向$n胸口的「玄机穴」",
	"damage_type" : "瘀伤",
	"lvl" : 50,
	"skill_name" : "鸳鸯腿"
]),
([
	"action" : "$N一招「仙鹤亮翅」，倏地斜穿闪到$n的身后，回手出掌，向$n的$l打到",
	"damage_type" : "瘀伤",
	"lvl" : 60,
	"skill_name" : "仙鹤亮翅"
]),
([
	"action" : "$N施出「白露横江」，左手猛击，待其闪避，突然右手抓住$n的$l，猛喝一声，将$n向外掼了出去",
	"damage_type" : "瘀伤",
	"lvl" : 90,
	"skill_name" : "白露横江"
]),
([
	"action" : "$N大吼一声，猛窜而起，左手伸出，勾向$n的手腕，接着右掌一招「白虹贯日」，使足全力向$n的$l击去",
	"damage_type" : "内伤",
	"lvl" : 120,
	"skill_name" : "白虹贯日"
]),
([
	"action" : "$N手腕一翻，使出一招「九转丹成」，转了一个圈子，双掌直推$n的$l",
	"damage_type" : "内伤",
	"lvl" : 150,
	"skill_name" : "九转丹成"
]),
([
	"action" : "$N气凝右臂，左臂稍曲，左掌在右肘一托，一招「横架金梁」，奋力挥掌往$n的$l拍去",
	"damage_type" : "内伤",
	"lvl" : 180,
	"skill_name" : "横架金梁"
])
});

string query_class() {return "正";}

int valid_enable(string usage) { return (usage == "strike") || (usage == "parry"); }

int valid_learn(object me)
{
	if( me->query_temp("weapon") || me->query_temp("secondary_weapon") )
		return notify_fail("练震山绵掌必须空手。\n");
	if( (int)me->query_skill("yinyun-ziqi", 1) < 100 )
		return notify_fail("你的氤氲紫气火候不够，无法学习震山绵掌。\n");
	if( (int)me->query("max_neili") < 900 )
		return notify_fail("你的内力太弱，无法练震山绵掌。\n");
	return 1;
}

int practice_skill(object me)
{
	int lvl = me->query_skill("zhenshan-mianzhang", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if( (int)me->query("jingli") < 40 )
		return notify_fail("你的精力太低了，无法练习震山绵掌\n");
	if( (int)me->query("neili") < 20 )
		return notify_fail("你的内力不够练震山绵掌。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
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

	level = (int)me->query_skill("zhenshan-mianzhang", 1);

	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action" : action[j]["action"],
				"damage_type": action[j]["damage_type"],
				"lvl":	action[j]["lvl"],
				"force" : 220 + random(300),
				"skill_class" 	: ZHENG,
				"skill_damage" 	: 30,
				"skill_attack" 	: 0,
				"skill_parry" 	: 0,
				"skill_dodge"		: 0,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : 20,			
				"skill_GP" 			: 2	
			]);
		}
	}
}

string perform_action_file(string action)
{
	return __DIR__"wd/zhenshan-mianzhang/" + action;
}
