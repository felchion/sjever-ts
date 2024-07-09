#include <ansi.h>
inherit SKILL;

mapping *action = ({
([
	"action" : HIB"$N使一招「风起云涌」，双手飘移不定击向$n$l"NOR,
	"skill_name" : "风起云涌",
	"lvl" : 10,
	"damage_type" : "瘀伤"
]),
([
	"action" : HIW"$N倏忽欺至$n身前，一招「扬波起浪」直拍$n的$l"NOR,
	"skill_name" : "扬波起浪",
	"lvl" : 30,
	"damage_type" : "瘀伤"
]),
([
	"action" :HIY "$N身形绕$n一转，一招「风雷并起」向$n$l接连出掌"NOR,
	"skill_name" : "风雷并起",
	"lvl" : 50,
	"damage_type" : "瘀伤"
]),
([
	"action" : HIR"$N身形拔起，在半空一招「急电惊雷」右掌猛击$n的$l"NOR,
	"skill_name" : "急电惊雷",
	"lvl" : 90,
	"damage_type" : "瘀伤"
]),
([
	"action" : HIC"$N使一招「含沙射影」，纵起丈余直击$n的$l"NOR,
	"skill_name" : "含沙射影",
	"lvl" : 120,
	"damage_type" : "瘀伤"
]),
([
	"action" : HIG"$N一声大喝使出「夺魄催心」，幻出满天掌影，掌掌不离$n的$l"NOR,
	"skill_name" : "夺魄催心",
	"lvl" : 150,
	"damage_type" : "瘀伤"
]),
});

int valid_enable(string usage) { return usage=="strike" || usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练九阴神掌必须空手。\n");
	if ((int)me->query_skill("jiuyin-zhengong",1) < 50)
		return notify_fail("你的九阴真功不够火候，无法练九阴神掌。\n");
	if ((int)me->query_skill("cuixin-zhang",1) > 1)
		return notify_fail("你学了催心掌，无法练九阴神掌。\n");
	if ((int)me->query("max_neili") < 1000)
		return notify_fail("你的内力太弱，无法练九阴神掌。\n");
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
	level = (int) me->query_skill("jiuyin-shenzhang",1);
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
		return notify_fail("你的内力不够练催心掌。\n");
	me->receive_damage("jingli", 50);
	me->add("neili", -20);
	return 1;
}

int valid_combine(string combo){return combo=="dafumo-quan";}

string perform_action_file(string action)
{
	return __DIR__"quest/jiuyin-shenzhang/" + action;
}

int hit_ob(object me, object ob, int damage)
{
	int i;
	i=me->query_skill("jiuyin-shenzhang")*(me->query_skill("jiuyin-shenzhang")/100+1);
	if(i>3000) i=3000+random(i-3000)/2;
	message_vision(HIR"$N被九阴真气一震，方寸大乱！\n"NOR,ob);
	if(damage>i)
		damage=i;//修改damage by tangfeng
	ob->add("jingli",-random(damage));
	ob->set("jiali",0);
	return 0;
}
