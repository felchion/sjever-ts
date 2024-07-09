// yanxing-daofa.c 雁行刀法

inherit SKILL;
#include  <ansi.h>
#include <skill_class.h>

string query_class() {return "正";}

mapping *action = ({
([
	"action" : "$N默念「让字诀」，迅速闪开$n的攻势，$w同时随身法向$n的前胸挥出！",
	"lvl" : 0,
	"skill_name" : "让字决",
	"damage_type" : "割伤"
]),
([
	"action" : "$N坐闪右让，一招「打字诀」，手中$w直出，出其不意地打向$n的后背！",
	"lvl" : 30,
	"skill_name" : "打字决",
	"damage_type" : "割伤"
]),
([
	"action" : "$N故示其弱，一招「引字诀」，乘$n信以为真时，猛地挥动$w向$n的右脚斩去！",
	"lvl" : 60,
	"skill_name" : "引字决",
	"damage_type" : "割伤"
]),
([
	"action" : "$N手捏「套字诀」，撤步套让，传身而走，诱$n追击，乘其不备向后猛击！",
	"lvl" : 80,
	"skill_name" : "套字决",
	"damage_type" : "割伤"
]),
([
	"action" : "$N刀锋暗收，一招「藏字诀」，刀光乍展，指上打下，指东打西，杀机无限！",
	"lvl" : 100,
	"skill_name" : "藏字决",
	"damage_type" : "割伤"
]),
([
	"action" : "$N影意守「错字诀」，步步玄机，招招错乱，$n顿时迷离失措，束手待毙！",
	"lvl" : 120,
	"skill_name" : "错字决",
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
	if ( me->query("max_neili") < 100)
		return notify_fail("你的内力不够。\n");
	if ( me->query_skill("linji-zhuang", 1) < 30)
		return notify_fail("你的临济十二庄火候太浅，无法学习雁行刀法。\n");
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
	level   = (int) me->query_skill("yanxing-daofa",1);
	for(i = sizeof(action); i > 0; i--)
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([  
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"skill_class"   : ZHENG,
	      "skill_damage"  : 10,
	      "skill_attack"  : 10,
	      "skill_parry"   : 20,
	      "skill_dodge"   : -10,
	      "skill_kill"    : -10,
	      "skill_rush"    : 10,
	      "skill_hurt"    : 20,
	      "skill_GP"      : 2
			]);
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("yanxing-daofa", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 40)
		return notify_fail("你的体力不够练雁行刀法。\n");
	if (me->query("neili") < 20)
		return notify_fail("你的内力不够练雁行刀法。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"em/yanxing-daofa/" + action;
}

void huanying(object me, object victim)
{
	if (!me || !victim || !me->is_fighting(victim))
		return;

	if( me->query_temp("幻影")
	 && !me->query_temp("huanying_hit")){
		me->set_temp("huanying_hit", 1);
		me->set_temp("apply/name", ({ me->query("name")+"的"HIY"幻影"NOR }));
		COMBAT_D->do_attack(me, victim, me->query_temp("weapon"), random(3)?1:3);
		me->delete_temp("apply/name");
		me->delete_temp("huanying_hit");
	}

}

mixed hit_ob(object me, object victim, int damage_bonus, int factor)
{
	call_out((: huanying :), 1, me, victim);
}
