inherit SKILL;

mapping *action = ({
([
	"action" : "$N深吸一口气，双掌缓缓的推向$n的$l",
	"lvl" : 0,
	"damage_type" : "内伤"
]),
([
	"action" : "$N左掌拍出，右掌后发先至，直逼向$n的$l",
	"lvl" : 30,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N一转身，右手反掌击出，待$n出手招架，左掌飞快的拍向$n的$l",
	"lvl" : 60,
	"damage_type" : "内伤"
]),
([
	"action" : "$N双掌齐出，一掌一掌带著阴寒之气不断的向$n攻去",
	"lvl" : 120,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N内息运转，周身泛起一片冷气，一掌拍向$n的$l",
	"lvl" : 180,
	"damage_type" : "瘀伤"
]),
});

int valid_enable(string usage)
{
	object me = this_player();
	if ( !userp(me) || me->query("menggu") || me->query("xmsz")){
		return usage=="strike" || usage=="parry";
	}
}

int valid_learn(object me)
{
	if (!me->query("menggu") && !me->query("xmsz"))
		return notify_fail("你现在不能练习玄冥神掌。\n");
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练玄冥神掌必须空手。\n");
	if ((int)me->query("max_neili") < 3000)
		return notify_fail("你的内力太弱，无法练功。\n");
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
	level   = (int) me->query_skill("xuanming-shenzhang",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
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
	if (!me->query("menggu") && !me->query("xmsz"))
		return notify_fail("你现在不能练习玄冥神掌。\n");
	if ((int)me->query("jingli") < 60)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 30)
		return notify_fail("你的内力不够练玄冥神掌。\n");
	me->receive_damage("jingli", 50);
	me->add("neili", -20);
	return 1;
}

mixed hit_ob(object me, object victim, int damage_bonus, int factor)
{
	int lvl;
	lvl = me->query_skill("xuanming-shenzhang")/5;
	if (!userp(me)) lvl /= 4;

	if ( victim->query("job_npc")) return 0;

	if (me->query("env/玄冥神掌") == 1
	&& random(me->query("neili") + victim->query("neili")) > victim->query("neili")) {
		victim->add_condition("xuanmin_poison", lvl);
		if ( userp(me) && userp(victim)
		 && victim->query("combat_exp") < me->query("combat_exp") / 2
		 && !victim->query_temp("kill_other/"+me->query("id"))
		 && !me->query_temp("other_kill/"+victim->query("id"))
		 && ! me->query_temp("revenge/"+victim->query("id"))
		 && !victim->query_condition("killer") )
			me->add_condition("killer", 5);
	}
	if (me->query("env/玄冥神掌") == 1
	&& random(me->query("neili") + victim->query("neili")) > me->query("neili")
	&& userp(victim)
	&& userp(me))
		me->add_condition("xuanmin_poison", lvl);
}

string perform_action_file(string action)
{
	return __DIR__"other/xuanming-shenzhang/" + action;
}
