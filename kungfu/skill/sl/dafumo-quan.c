// dafumo-quan.c
#include <ansi.h>
inherit SKILL;

mapping *action = ({
([
	"action" : HIY"$N双掌一分，抢到$n的面前，突然使出「云映晴空」，双拳袭向$n"NOR,
	"lvl" : 0,
	"skill_name" : "云映晴空",
	"damage_type" : "瘀伤"
]),
([
	"action" : HIG"$N使出「九重天开」，变拳为掌，双掌并力推向$n"NOR,
	"lvl" : 7,
	"skill_name" : "九重天开",
	"damage_type" : "瘀伤"
]),
([
	"action" : HIR"$N一跃至$n身前，「风动天庭」，一拳击向$n"NOR,
	"lvl" : 15,
	"skill_name" : "风动天庭",
	"damage_type" : "瘀伤"
]),
([
	"action" : HIB"$N只攻不守，门户大开，一招「满目苍生」攻向$n"NOR,
	"lvl" : 28,
	"skill_name" : "满目苍生",
	"damage_type" : "瘀伤"
]),
([
	"action" : HIW"$N双拳舞动如飞，一式「空王不语」扫向$n的下盘"NOR,
	"lvl" : 45,
	"skill_name" : "空王不语",
	"damage_type" : "瘀伤"
]),
([
	"action" : HIC"$N身形拔起，劈空而下，「拳掠横空」击向$n"NOR,
	"lvl" : 60,
	"skill_name" : "拳掠横空",
	"damage_type" : "瘀伤"
]),
([
	"action" : HIM"$N身形身形稳稳站定，双拳齐挥，一招「青山云海」袭向$n"NOR,
	"lvl" : 65,
	"skill_name" : "青山云海",
	"damage_type" : "瘀伤"
]),
([
	"action" : MAG"$N左掌画了个圈圈，「狂风扫叶」，右拳打出，击向$n"NOR,
	"lvl" : 70,
	"skill_name" : "狂风扫叶",
	"damage_type" : "瘀伤"
]),
([
	"action" : YEL"$N大开大合，施展出「苦海慈航」，双拳撞向$n的前胸"NOR,
	"lvl" : 80,
	"skill_name" : "苦海慈航",
	"damage_type" : "瘀伤"
]),
});

int valid_enable(string usage) { return usage=="cuff" || usage=="parry"; }

int valid_combine(string combo) { return combo=="jiuyin-shenzhang"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("学大伏魔拳法必须空手。\n");
	if ((int)me->query_skill("jiuyin-zhengong", 1) <= (int)me->query_skill("dafumo-quan", 1))
		return notify_fail("你的九阴真功火候不够，无法学大伏魔拳法。\n");
	if ( me->query_skill("dafumo-quan", 1) < 150 )
		return notify_fail("你的大伏魔拳等级还不够练习。\n");
	if (me->query_skill_mapped("force") != "jiuyin-zhengong")
		return notify_fail("修习大伏魔拳法必须有九阴真功配合。\n");
	if ((int)me->query("max_neili") < 1000)
		return notify_fail("你的内力太弱，无法练大伏魔拳法。\n");
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
	level   = (int) me->query_skill("dafumo-quan",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level >= action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action":	action[j]["action"],
				"lvl":		action[j]["lvl"],
				"damage_type":	action[j]["damage_type"],
				"force":	300 + random(350),
				"dodge":	random(30),
				"parry":	random(30),
			]);
		}
	}
}

int practice_skill(object me)
{
	if ((int)me->query("jingli") < 60)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 30)
		return notify_fail("你的内力不够练大伏魔拳法。\n");
	me->receive_damage("jingli", 50);
	me->add("neili", -20);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"sl/dafumo-quan/" + action;
}

int hit_ob(object me, object ob, int damage)
{
	int i;
	i=me->query_skill("jiuyin-shenzhang")*(me->query_skill("jiuyin-shenzhang")/100+1);
	if(i>3000) i=3000+random(i-3000)/2;
	if(damage>i)
		damage=i;//修改damage by tangfeng
	message_vision(HIR"$N被九阴真气一震，内息大乱！\n"NOR,ob);
	ob->add("neili",-random(damage));
	return 0;
}
