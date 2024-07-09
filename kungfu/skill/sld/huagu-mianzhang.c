// huagu-mianzhang.c 化骨绵掌
// rewrite by olives@SJ
// 4/19/2000

inherit SKILL;
#include <skill_class.h>
#include <ansi.h>

string query_class() {return "柔";}

mapping *action = ({
([
	"action" : "只见$N身形展动，绕着$n不停转圈，身法奇快，一式「春风拂柳」，一掌又一掌地向$n身上击去",
	"damage_type" : "内伤",
	"lvl" : 0,
	"skill_name" : "春风拂柳"
]),
([
	"action" : "$N双掌一立，一式「灵蛇出洞」，右手化掌为抓，噗的一声，挟着一股劲风向$n的猛抓过去",
	"damage_type" : "刺伤",
	"lvl" : 15,
	"skill_name" : "灵蛇出洞"
]),
([
	"action" : "$N缓缓地举起双掌，一式「轻风细雨」，不带一点掌力地轻轻地拍向$n的前胸",
	"damage_type" : "内伤",
	"lvl" : 30,
	"skill_name" : "轻风细雨"
]),
([
	"action" : "只见$N一肩转身，左手护面，右手伏击，乘$n不备，一式「风行草偃」，猛地一掌击出",
	"damage_type" : "瘀伤",
	"lvl" : 45,
	"skill_name" : "风行草偃"
]),
([
	"action" : "$N右掌略偏，在$n左肩轻轻一按，然后左掌向穿出，一式「半雨半晴」，向$n的$l轻轻拍了下去",
	"damage_type" : "瘀伤",
	"lvl" : 60,
	"skill_name" : "半雨半晴"
]),
([
	"action" : "$N身形猛地向上跃起，一式「如影随形」，双臂柔若无骨般地缠上$n的双臂，然后双掌一合，轻飘飘地向$n前胸拍去",
	"damage_type" : "瘀伤",
	"lvl" : 80,
	"skill_name" : "如影随形"
]),
});

int valid_enable(string usage) { return usage=="strike" ||  usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练化骨绵掌必须空手。\n");
	if ((int)me->query_skill("dulong-dafa", 1) < 30)
		return notify_fail("你的毒龙大法火候不够，无法学化骨绵掌。\n");
	if ((int)me->query("max_neili") < 250)
		return notify_fail("你的内力太弱，无法练化骨绵掌。\n");
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
	level = (int)me->query_skill("huagu-mianzhang", 1);

	for(i = sizeof(action); i > 0; i--) {
		if(level >= action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);						
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
				"force"					: 350 +random(150),
				"skill_class" 	: ROU,
				"skill_damage" 	: -10,
				"skill_attack" 	: 20,
				"skill_parry" 	: -10,
				"skill_dodge"		: 10,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : 40,
				"skill_GP" 			: 2	
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("huagu-mianzhang", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 50)
		return notify_fail("你的精力太低了。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练化骨绵掌。\n");
	me->receive_damage("jingli", 40 );
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{   
	return __DIR__"sld/huagu-mianzhang/" + action;	   
}

void skill_improved(object me)
{
	int skill, yl;
	skill = me->query_skill("huagu-mianzhang", 1);
	yl = me->query_skill("shenlong-yaoli", 1);
	if( skill >= 200 && !me->query("sld/hgmz") ){
		tell_object(me, HIW"\n忽然，你觉得心中一阵畅然，浑身只觉精力弥漫，有使不完的劲，不禁长啸起来。！\n"NOR);
		me->add("max_neili", random(yl+random(yl)));
		me->set("sld/hgmz", 1);
	}
}
