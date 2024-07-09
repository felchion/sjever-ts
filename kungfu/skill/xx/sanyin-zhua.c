//  sanyin-zhua 三阴蜈蚣爪
// by dubei

#include <ansi.h>
inherit SKILL;

mapping *action = ({
([
	"action": "$N一式「蜈蚣盘步」，右手虚晃，左手化爪，连晃几下，突然向$n的背后二穴抓去",
	"lvl" : 0,
	"damage_type": "抓伤"
]),
([
	"action": "$N侧身一晃，十指飞舞，挟带一股从指尖冒出的阴气向$n的$l连连插去",
	"lvl" : 10,
	"weapon" : "指风",
	"damage_type": "刺伤"
]),
([
	"action": "$N一式「毒蚣盘身」，右手爪指交错，手臂一挥，出手就扣向$n的咽喉要害",
	"lvl" : 20,
	"damage_type": "抓伤"
]),
([
	"action": "$N错步飘出，迷幻$n，接着右手斜斜探出，阴狠的抓向$n的$l",
        "weapon" : "利爪",
	"lvl" : 40,
	"damage_type": "抓伤"
]),
([
	"action": "$N一式「蜈蚣摆尾」，十指伸缩，泛起森森蓝光，虚虚实实地袭向$n的全身要穴",
	"lvl" : 60,
	"damage_type": "内伤"
]),
([
	"action": "$N面显阴笑，腾身飞出，十指若隐若现，若有若无，急急地抓向$n的$l",
	"lvl" : 80,
	"damage_type": "抓伤"
]),
([
	"action": "$N惨笑一声，人如中风发狂一般，双臂连连舞动，卷起满天爪影直袭$n的$l",
        "weapon" : "手爪",
	"lvl" : 100,
	"damage_type": "抓伤"
]),
});

int valid_enable(string usage) { return usage=="claw" || usage=="parry"; }

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
	level = (int)me->query_skill("sanyin-zhua",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([  
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"weapon": action[j]["weapon"],
				"force":150+random(300),
				"dodge":random(30),
				"parry":random(30),
			]);
		}
	}
}

int valid_learn(object me)
{
	if( me->query_temp("weapon"))
		return notify_fail("你手里有兵器，如何练习三阴蜈蚣爪？\n");
	if ( me->query_skill("huagong-dafa", 1) < 10)
		return notify_fail("你的化功大法等级不够。\n");
	if ( me->query("max_neili") < 100 )
		return notify_fail("你的内力不够。\n");
	return 1;
}

int practice_skill(object me)
{
	int lvl = me->query_skill("sanyin-zhua", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 40)
		return notify_fail("你的体力太低了。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够，无法练三阴蜈蚣爪。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

mixed hit_ob(object me, object victim, int damage_bonus, int factor)
{
	int poison, i;

	poison = (me->query_skill("poison", 1) - victim->query_skill("poison", 1))/5;
	poison += me->query_skill("sanyin-zhua",1)/10;

	if ( userp(victim)) {
		poison /= 3;
		i = 160;
	}
	else
		i = 80;

	if(random(me->query_skill("huagong-dafa", 1)) > i
	&& me->query_skill("sanyin-zhua", 1) > 100
	&& me->query_skill("poison", 1) > 50
	&& me->query("env/化功大法") != "内敛"
	&& random(me->query_skill("poison", 1)) > random(victim->query_skill("medicine", 1))
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
		if (userp(me)
		 && userp(victim)
		 && !victim->query_temp("kill_other/"+me->query("id"))
		 && !me->query_temp("other_kill/"+victim->query("id"))
		 && !me->query_temp("revenge/"+victim->query("id"))
		 && !victim->query_condition("killer") )
			me->add_condition("killer", 5);
	}
}

string perform_action_file(string action)
{
	return __DIR__"./xx/sanyin-zhua/" + action;
}
