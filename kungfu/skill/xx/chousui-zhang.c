// chousui-zhang.c, 抽髓掌
// by pishou
// Modified by Lklv 2002.1.30

#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

//string query_class() {return "险";}

mapping *action = ({
([
	"action": "$N催动内力，衣袖挥动，双掌挟着一股腥臭之气一前一后拍向$n的$l",
	"lvl" : 0,
	"damage_type": "瘀伤"
]),
([
	"action": "$N双脚一曲一弯，身体旋转起来, 突然间双掌飞舞着拍向$n的$l",
	"lvl" : 20,
	"damage_type": "瘀伤"
]),
([
	"action": "$N身形飘动，围着$n一圈圈游走，同时双掌连拍，激起阵阵劲风吹向$n",
	"lvl" : 40,
	"damage_type": "内伤"
]),
([
	"action": "$N突然厉声大喝，跃于半空，立掌如斧，上上下下向$n连连劈出数掌",
	"weapon" : "掌风",
	"lvl" : 60,
	"damage_type": "割伤"
]),
([
	"action": "$N身形微晁抢前一步，运起十成功力，大喝一声，须发戟张，呼的一掌又向$n推去",
	"lvl" : 90,
	"damage_type": "内伤"
]),
([
	"action": "$N面露诡异的笑容，猛地向前冲出，对准$n的$l就是一招阴毒无比的「抽髓掌」",
	"lvl" : 120,
	"damage_type": "瘀伤"
]),
([
	"action": "$N心念一转，计上心来，一招「三阴蜈蚣爪」，招招连环，连珠般抓向$n的$l",
	"weapon" : "手爪",
	"lvl" : 150,
	"damage_type": "抓伤"
]),
});

int valid_enable(string usage) { return usage=="strike" || usage=="parry"; }

string query_skill_name(int level)
{
	int i = sizeof(action);
	while (i--)
		if(level > action[i]["lvl"])
			return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
	int i, j,level;
  
  level = (int)me->query_skill("chousui-zhang", 1);
	for(i = sizeof(action); i > 0; i--)
		if(level >= action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action" : action[j]["action"],
				"lvl" : action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
	//			"skill_class" 	: XIAN,
				"skill_damage" 	: 30 ,
				"skill_attack" 	: 0 ,
				"skill_parry" 	: 0 ,
				"skill_dodge"		: 20 ,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : 0,
				"skill_GP" 			: 2
			]);
		}
}

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("空了手才能练抽髓掌。\n");
	if ((int)me->query_skill("huagong-dafa", 1) < 10)
		return notify_fail("你还是先练好化功大法吧。\n");
	if ((int)me->query("max_neili") < 100)
		return notify_fail("你内力修行太差, 练不了抽髓掌。\n");
	return 1;
}

int practice_skill(object me)
{
	int lvl = me->query_skill("chousui-zhang", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 50)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练抽髓掌。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

mixed hit_ob(object me, object victim, int damage_bonus, int factor)
{
	int poison, i;

	poison = (me->query_skill("poison", 1) - victim->query_skill("poison", 1))/5;
	poison += me->query_skill("chousui-zhang",1)/10;

	if ( userp(victim)) {
		poison /= 3;
		i = 120;
	}
	else
		i = 50;


	if(random(me->query_skill("huagong-dafa", 1)) > i
	&& me->query_skill("chousui-zhang", 1) > 50
	&& me->query_skill("poison", 1) > 50
	&& me->query("env/化功大法") != "内敛"
	&& random(me->query_skill("poison", 1)) > random( victim->query_skill("medicine", 1))
	&& victim->query_condition("xx_poison") < 200
	&& me->query("neili") > 200
	&& !victim->query("job_npc")
	&& !userp(victim)) {
		if (me->query("env/化功大法") == "散毒") {
			poison += random(poison/2);
			me->add("neili",-100-random(50));
			me->add("jingli",-20-random(30));
			message_vision(BLU"$N内力一催，手爪上蓝气大盛！\n"NOR, me);
		}
		victim->add_condition("xx_poison", poison);
		message_vision(BLU"$N一中爪，身子一晃，看来已经中了毒！\n"NOR, victim);
		if (victim->query_condition("xx_poison") > 180)
			message_vision(HBRED"$N体内积毒已深，恐怕有生命危险！\n"NOR, victim);
	}
}

string perform_action_file(string action)
{
	return __DIR__"xx/chousui-zhang/" + action;
}
