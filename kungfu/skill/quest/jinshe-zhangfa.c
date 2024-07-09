//jinshe-zhangfa.c

inherit SKILL;

mapping *action = ({
([
	"action": "$N使一招「金蛇出洞」，双手轻摆，宛如灵蛇出洞飘移不定击向$n$l",
        "lvl" : 0,
        "damage_type": "瘀伤"
]),
([
	"action": "$N使一招「金蛇游水」，双臂微弯如蛇行，手掌立起拍向$n的$l",
        "lvl" : 20,
        "damage_type": "瘀伤"
]),
([
	"action": "$N突然微一弓身，一招「金蛇挺身」，双掌划向$n的$l",
        "lvl" : 40, 
        "damage_type": "瘀伤"
]),
([
	"action": "$N倏忽欺至$n身前，一招「金蛇过涧」直拍$n的$l",
        "lvl" : 60,
        "damage_type": "瘀伤"
]),
([
	"action": "$N身形绕$n一转，一招「金蛇缠身」向$n$l接连出掌",
        "lvl" : 70, 
        "damage_type": "瘀伤"
]),
([
	"action": "$N身形拔起，在半空一招「升天入地」右掌猛击$n的$l",
        "lvl" : 80, 
        "damage_type": "瘀伤"
]),
([
	"action": "$N使一招「金蛇翻腾」，纵起丈余双掌交错直击$n的$l",
        "lvl" : 90,
        "damage_type": "瘀伤"
]),
([
	"action": "$N一声大喝使出「蛇跨彩虹」，幻出满天掌影，掌掌不离$n的$l",
        "lvl" : 100,
	"damage_type": "瘀伤"
]),
});

int valid_enable(string usage) { return usage=="strike" ||  usage=="parry" ||  usage=="dodge";; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练金蛇游身掌必须空手。\n");
	if ((int)me->query_skill("strike", 1) < 60)
		return notify_fail("你的基本功不够，不能学习金蛇游身掌。\n");
	if( !me->query("jinshe/zhangfa"))
		return notify_fail("你无法修习金蛇游身掌。\n");
	if ((int)me->query("max_neili") < 1000)
		return notify_fail("你的内力太弱，无法练金蛇游身掌。\n");
	return 1;
}

string query_skill_name(int level)
{
	int i = sizeof(action);
	while (i--)
		if(level >= action[i]["lvl"])
			return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
	int i, level, j;
	level   = (int) me->query_skill("jinshe-zhangfa",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action":	action[j]["action"],
				"lvl":		action[j]["lvl"],
				"damage_type":	action[j]["damage_type"],
				"force":	150 + random(300),
				"dodge":	random(30),
				"parry":	random(30),
			]);
		}
	}
}

int practice_skill(object me)
{
	if ((int)me->query("jingli") < 40)
		return notify_fail("你太累了。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练金蛇游身掌。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}
