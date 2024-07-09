// fengmo-zhang.c 疯魔杖法
// by hunthu
// update by cool
// Lklv update 2001.10.05

#include <ansi.h>
inherit SKILL;

mapping *action = ({
([
	"action": "$N斜举$w, 一招「秦王鞭石」，斜肩铲背砸向$n的$l",
	"lvl" : 0,
	"skill_name" : "秦王鞭石",
	"damage_type": "挫伤"
]),
([
	"action": "$N一声大喝，使出一招「鲁达拔柳」, 手中$w自下而上扫向$n的$l",
	"lvl" : 20,
	"skill_name" : "鲁达拔柳",
	"damage_type": "挫伤"
]),
([
	"action": "$N使出一招「霸王扛鼎」，将$w从肩头甩出砸向往$n的$l",
	"lvl" : 40,
	"skill_name" : "霸王扛鼎",
	"damage_type": "瘀伤"
]),
([
	"action": "$N身子一晃，一招「八仙醉打」，手中$w如狂风暴雨般向$n打出",
	"lvl" : 60,
	"skill_name" : "八仙醉打",
	"damage_type": "瘀伤"
]),
([
	"action": "$N一声咆哮，一招「长虹经天」, $w从手中掷出，飞向$n的$l",
	"lvl" : 80,
	"skill_name" : "长虹经天",
	"damage_type": "瘀伤"
]),
});

string query_skill_name(int level)
{
	int i = sizeof(action);
	while (i--)
		if(level > action[i]["lvl"])
			return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
	int i, level, j;

	if( me->query_skill("staff", 1) >= 120
	 && me->query_skill("fengmo-zhang", 1) >= 120
	 && me->query_skill("force",1) >= 100
	 && me->query("neili") >= 500
	 && random(me->query_str(1)) >= 25
	 && me->query("shen")+1000 < 0 && random(2) ) {
		return ([
			"action": RED"$N脸上突现煞气，举起$w，猛向$n後脑击落。这一杖出手又快又狠，竟是「伏魔杖法」中的毒招"NOR,
			"damage": 50+random(60),
			"parry": random(15),
			"dodge": random(20) - 5,
			"force": 110+random(250),
			"damage_type": "内伤"
		]);
	}
	level = (int) me->query_skill("fengmo-zhang",1);
	for(i = sizeof(action); i > 0; i--)
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([  
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"force":100+random(250),
				"dodge":random(20)-10,
				"damage":40+random(60),
				"parry":random(10),
			]);
		}
}

int valid_enable(string usage) { return (usage == "staff") || (usage == "parry"); }

int valid_learn(object me)
{
	object weapon;
	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "staff")
		return notify_fail("你手上的武器不能用来练疯魔杖法。\n");
	if(me->query_skill("huntian-qigong", 1) < 20)
		return notify_fail("你的混天气功等级不够。\n");
	if(me->query("max_neili") < 200)
		return notify_fail("你还是先集中精力修练内力吧。\n");
	return 1;
}

int practice_skill(object me)
{
	int lvl = me->query_skill("fengmo-zhang", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if (me->query("jingli") < 30)
		return notify_fail("你先歇口气再练吧。\n");
	if( me->query("neili") < 15)
		return notify_fail("你的内力不够练疯魔杖法。。\n");
	me->receive_damage("jingli", 20);
	me->add("neili", -5);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"gb/fengmo-zhang/" + action;
}
