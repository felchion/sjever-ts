// qianye-shou.c -如来千叶手
#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

string query_class() {return "奇";} 

mapping *action = ({
([
	"action" : "$N一式「南海礼佛」，右手虚晃，左手扬起，突然拍向$n的背后二穴",
	"lvl" : 0,
	"skill_name" : "南海礼佛",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N侧身一晃，一式「金玉瓦砾」，左手拿向$n的肩头，右拳打向$n的$l",
	"lvl" : 20,
	"skill_name" : "金玉瓦砾",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N一式「人命呼吸」，右手环拢成爪，一出手就向扣$n的咽喉要害",
	"lvl" : 40,
	"skill_name" : "人命呼吸",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N左手虚招，右掌直立，一式「镜里观影」，错步飘出，疾拍$n的$l",
	"lvl" : 60,
	"skill_name" : "镜里观影",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N使一式「水中捉月」，左拳上格，右手探底突出，抓向$n的$l",
	"lvl" : 80,
	"skill_name" : "水中捉月",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N双拳挥舞，一式「浮云去来」，两手环扣，拢成圈状，猛击$n的$l",
	"lvl" : 100,
	"skill_name" : "浮云去来",
	"damage_type" : "内伤"
]),
([
	"action" : "$N一式「水泡出没」，十指伸缩，虚虚实实地袭向$n的全身要穴",
	"lvl" : 120,
	"skill_name" : "水泡出没",
	"damage_type" : "内伤"
]),
([
	"action" : "$N双手抱拳，一式「梦里明明」，掌影翻飞，同时向$n施出九九八十一招",
	"lvl" : 150,
	"skill_name" : "梦里明明",
	"damage_type" : "内伤"
]),
([
	"action" : "$N一式「觉后空空」，拳招若隐若现，若有若无，缓缓地拍向$n的丹田",
	"lvl" : 180,
	"skill_name" : "觉后空空",
	"damage_type" : "内伤"
]),
});

int valid_enable(string usage) { return usage=="hand" || usage=="parry"; }


int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练如来千叶手必须空手。\n");
	if ((int)me->query_skill("yijin-jing", 1) < 60)
		return notify_fail("你的易筋经内功火候不够，无法学如来千叶手。\n");
	if ((int)me->query_int(1)>30) {
		return 1;
	}
	else {
		if ( me->query_skill("banruo-zhang", 1)
		 && me->query_skill("yizhi-chan", 1)
		 && me->query_skill("jingang-quan", 1)
		 && me->query_skill("nianhua-zhi", 1)
		 && me->query_skill("longzhua-gong", 1)
		 && me->query_skill("ruying-suixingtui", 1)
		 && me->query_skill("sanhua-zhang", 1))
			return notify_fail("你的功力不够，无法同时学习两种以上的上乘功夫。\n");
        }

	if ((int)me->query("max_neili") < 600)
		return notify_fail("你的内力太弱，无法练如来千叶手。\n");
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
	level   = (int) me->query_skill("qianye-shou",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"damage_type": action[j]["damage_type"],
				"lvl": action[j]["lvl"],
				"skill_class" 	: QI,
				"skill_damage" 	: 10 ,
				"skill_attack" 	: 0 ,
				"skill_parry" 	: 20 ,
				"skill_dodge"		: 20 ,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : 0,
				"skill_GP" 			: 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("qianye-shou", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 40)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练如来千叶手。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

mixed hit_ob(object me, object target,int damage_bonus)
{
	int j = me->query_skill("qianye-shou", 1);

	if( !random(3)
	&& !target->is_busy()
	&& j >= random(150)
	&& me->query_skill("buddhism",1) >= random(180)
	&& me->query("neili") >= 1000 
	&& me->query("max_neili") >= 2500 
	&& me->query("combat_exp")/2 > random(target->query("combat_exp"))){
		message_vision(CYN"掌到中途，忽然微微摇晃，登时一掌变两掌，两掌变四掌，四掌变八掌，结果$n迟得顷刻，\n"
			"$N便八掌变十六掌，进而幻化为三十二掌，$n躲闪不及被漫天掌影裹在核心。\n"NOR, me, target);
		return damage_bonus*(j/10+random(j))/400;
	}
	return 0;
}

string perform_action_file(string action)
{
	return __DIR__"sl/qianye-shou/" + action;
}
