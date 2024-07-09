// songshan-jian.c  嵩山剑法
// Lklv@SJ modified 2001.10.22
// tangfeng@SJ 2004.12

#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

string query_class() {return "快";}

mapping *action = ({
([
	"action" : "$N举剑过顶，弯腰躬身，恭敬一式「万岳朝宗」，$w向$n的$l轻轻刺去，",
	"lvl" : 0,
	"damage_type" : "刺伤"
]),
([
	"action" : "突然间剑光一吐，$w化作一道白虹，$N使出一式「千古人龙」，剑气古朴，$w直刺$n的$l",
	"lvl" : 15,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N左手向外一分，右手$w向右掠出一招「开门见山」，剑气森然地向$n的$l挥去",
	"lvl" : 30,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N一剑从空中疾劈而下，招式看似平平无奇，但呼的一声响，实有石破天惊的气势，一招「独劈华山」$w如虹地刺向$n的$l",
	"lvl" : 45,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N一柄$w自半空中横过，剑身似曲似直，如一件活物一般，一式「天外玉龙」奔腾矫夭，气势雄浑地斩向$n的$l",
	"lvl" : 60,
	"damage_type" : "割伤"
]),
([
	"action" : "$N威仪整肃一式「玉井天池」，$w轰然一动，犹如千军万马奔驰而来，疾疾刺向$n的$l",
	"lvl" : 75,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N剑转一式「叠翠浮青」，俨然长枪大戟，黄沙千里，$w电闪雷鸣，将$n笼罩于重重剑气之中",
	"lvl" : 90,
	"damage_type" : "刺伤"
]),
([
	"action" : "$N剑尖向下，一招「龙啸九天」，$w忽然急转而上，点出的剑气将$n上身要害团团围住",
	"lvl" : 120,
	"damage_type" : "刺伤"
]),
});

int valid_enable(string usage) { return (usage == "sword") || (usage == "parry"); }

int valid_learn(object me)
{
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "sword")
		return notify_fail("你使用的武器不对。\n");
	if ((int)me->query("max_neili") < 100)
		return notify_fail("你的内力不够。\n");
	if ((int)me->query_skill("hanbing-zhenqi", 1) < 20)
		return notify_fail("你的寒冰真气火候太浅。\n");
	return 1;
}

mapping query_action(object me, object weapon)
{
	int i, level, j;
	level = me->query_skill("songshan-jian",1);	
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
  		return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
				"skill_class" 	: KUAI,
				"skill_damage" 	: 10,
				"skill_attack" 	: 0,
				"skill_parry" 	: 0,
				"skill_dodge"		: 20,
				"skill_kill" 		: 30,
				"skill_rush" 		: 30,
				"skill_hurt" 	  : 0,
				"skill_GP" 			: 2		
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("songshan-jian", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;
	if ((int)me->query("jingli") < 50)
		return notify_fail("你的体力不够练嵩山剑法。\n");
	if ( me->query("neili") < 20 )
		return notify_fail("你的内力不够练嵩山剑法。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"ss/songshan-jian/" + action;
}
