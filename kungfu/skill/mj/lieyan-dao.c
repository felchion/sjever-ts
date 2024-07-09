// lieyan-dao.c 烈焰刀

inherit SKILL;
#include <skill_class.h>

string query_class() {return "快";}

mapping *action = ({
([
	"action" : "$N一招「黑龙现身」，$w有如一条黑龙在$n的周身旋游，勿快勿慢，变化若神",
	"lvl" : 0,
	"skill_name" : "黑龙现身",
	"damage_type" : "割伤"
]),
([
	"action" : "$N一招「万水千山」，左右腿一前一后，$w乱披风势向$n的$l斩去",
	"lvl" : 10,
	"skill_name" : "万水千山",
	"damage_type" : "割伤"
]),
([
	"action" : "$N纵身跃落，一招「横扫千里」，$w带着疾风呼的一声便向$n横扫过去",
	"lvl" : 20,
	"skill_name" : "横扫千里",
	"damage_type" : "割伤"
]),
([
	"action" : "$N一招「左右开弓」，$w大开大阖，左右并进，左一刀，右一刀向$n的两肩砍落",
	"lvl" : 30,
	"skill_name" : "左右开弓",
	"damage_type" : "割伤"
]),
([
	"action" : "$N手中$w自上而下，一招「百丈飞瀑」，刀光流泻，如一片雪白瀑布砍向$n的头部",
	"lvl" : 40,
	"skill_name" : "百丈飞瀑",
	"damage_type" : "割伤"
]),
([
	"action" : "$N挥舞$w，使出一招「直摧万马」，上劈下撩，左挡右开，如千军万马般罩向$n",
	"lvl" : 50,
	"skill_name" : "直摧万马",
	"damage_type" : "割伤"
]),
([
	"action" : "$N带得刀风劲疾，一招「怪蟒翻身」，一个猛转身，连刀带人往$n的$l的劈去",
	"lvl" : 60,
	"skill_name" : "怪蟒翻身",
	"damage_type" : "割伤"
]),
([
	"action" : "$N向前一个弓步，一招「上步劈山」，$w直直的劈出，一片流光般的刀影向$n的全身罩去",
	"lvl" : 70,
	"skill_name" : "上步劈山",
	"damage_type" : "割伤"
]),
});

int valid_enable(string usage) { return usage == "blade" || usage == "parry"; }

int valid_learn(object me)
{
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "blade")
		return notify_fail("你使用的武器不对。\n");
	if (me->query("max_neili") < 100)
		return notify_fail("你的内力不够。\n");
	if ( !me->query_skill("jiuyang-shengong", 1))
	if (me->query_skill("shenghuo-shengong", 1) < 20)
		return notify_fail("你的圣火神功火候太浅。\n");
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
	level   = (int) me->query_skill("lieyan-dao",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
				"skill_class" 	: KUAI,
				"skill_damage" 	: 0 ,
				"skill_attack" 	: 20 ,
				"skill_parry" 	: -30 ,
				"skill_dodge"		: 30 ,
				"skill_kill" 		: 0,
				"skill_rush" 		: 30,
				"skill_hurt" 	  : 0,
				"skill_GP" 			: 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("lieyan-dao", 1);
	int i = sizeof(action);
	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 30 || me->query("neili") < 10 )
	  return notify_fail("你的体力不够练烈焰刀。\n");
	me->receive_damage("jingli", 20);
	me->add("neili", -5);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"mj/lieyan-dao/" + action;
}
