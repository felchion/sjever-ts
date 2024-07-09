//    /kungfu/skill/suixing-tui.c 如影随形腿

#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

string query_class() {return "快";}

mapping *action = ({
([
	"action" : "$N纵身向前，忽然伸出左腿，一式「仗义执言」，直踢$n的头部",
	"lvl" : 0,
	"skill_name" : "仗义执言",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N身形一闪，双足点地，一式「七星伴月」，在空中连踢七脚，直本$n的头、胸、臂",
	"lvl" : 40,
	"skill_name" : "七星伴月",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N身体前倾，左脚画圆，右腿使出一式「佛界无边」，扫向$n的腰部",
	"lvl" : 80,
	"skill_name" : "佛界无边",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N左足倏地弹出，连环六腿，分踢$n的头部，胸部和裆部，正是一式「转世轮回」",
	"lvl" : 120,
	"skill_name" : "转世轮回",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N左足独立，右腿随身形反转横扫，一招「西天极乐」，踢向$n的$l",
	"lvl" : 150,
	"skill_name" : "西天极乐",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N跃起在半空，双足带起无数劲风，一式「佛祖慈悲」迅捷无伦地卷向$n",
	"lvl" : 180,
	"skill_name" : "佛祖慈悲",
	"damage_type" : "瘀伤"
]),
});

int valid_enable(string usage) { return usage=="leg" || usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("学如影随形腿时手里不能拿武器。\n");
	if ( me->query_skill("yijin-jing", 1) < 60)
		return notify_fail("你的易筋经内功火候不够，无法学如影随形腿。\n");
	if ( me->query_int(1) > 30)
		return 1;
	else {
		if ( me->query_skill("banruo-zhang", 1)
		 && me->query_skill("yizhi-chan", 1)
		 && me->query_skill("jingang-quan", 1)
		 && me->query_skill("nianhua-zhi", 1)
		 && me->query_skill("qianye-shou", 1)
		 && me->query_skill("longzhua-gong", 1)
		 && me->query_skill("sanhua-zhang", 1))
			return notify_fail("你的功力不够，无法同时学习两种以上的上乘功夫。\n");
	}

	if ((int)me->query("max_neili") < 600)
		return notify_fail("你的内力太弱，无法练如影随形腿。\n");
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
	level   = (int) me->query_skill("ruying-suixingtui",1);
        
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"damage_type": action[j]["damage_type"],
				"lvl": action[j]["lvl"],
				"skill_class" 	: KUAI,
				"skill_damage" 	: -10 ,
				"skill_attack" 	: 20 ,
				"skill_parry" 	: 0 ,
				"skill_dodge"		: 20 ,
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
	int lvl = me->query_skill("ruying-suixingtui", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 40)
		return notify_fail("你的精力太低了。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练如影随形腿。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"sl/ruying-suixingtui/" + action;
}

