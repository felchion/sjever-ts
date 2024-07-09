// liangyi-jian.c (kunlun) 昆仑派正两仪剑法
//update by cool 981104

#include <ansi.h>
inherit SKILL;

mapping *action = ({
([
	"action":"$N一式「顺水推舟」，$N手中$w指向自己左胸口，剑柄斜斜向右外，缓缓划向$n的$l",
	"lvl" : 0,
	"skill_name" : "顺水推舟",
	"damage_type":"刺伤"
]),
([
	"action":"$N身形微侧，左手后摆，右手$w一招「横扫千军」，直向$n的腰间挥去",
	"lvl" : 10,
	"skill_name" : "横扫千军",
	"damage_type":"刺伤"
]),
([
	"action":"$N纵身近前，$w斗然弯弯弹出，剑光爆长，一招「峭壁断云」，猛地刺向$n的胸口",
	"lvl" : 20,
	"skill_name" : "峭壁断云",
	"damage_type":"刺伤"
]),
([
	"action":"$N左手捏个剑决，平推而出，决指上仰，右手剑朝天不动，一招「仙人指路」，刺向$n",
	"lvl" : 40,
	"skill_name" : "仙人指路",
	"damage_type":"刺伤"
]),
([
	"action":"$N剑招忽变，使出一招「雨打飞花」，全走斜势，但七八招斜势中偶尔又挟着一招正势，教人极难捉摸",
	"lvl" : 60,
	"skill_name" : "雨打飞花",
	"damage_type":"刺伤"
]),
([
	"action":"$N手中$w剑刃竖起，锋口向下，一招「大漠平沙」，剑走刀势，劈向$n的$l",
	"lvl" : 80,
	"skill_name" : "大漠平沙",
	"damage_type":"劈伤"
]),
([
	"action":"$N一招「木叶萧萧」，$N横提$w，剑尖斜指向天，由上而下，劈向$n的$l",
	"lvl" : 100,
	"skill_name" : "木叶萧萧",
	"damage_type":"劈伤"
]),
([
	"action":"$N抢前一步，$w微微抖动，剑光点点，一招「江河不竭」，终而复始，绵绵不绝刺向$n",
	"lvl" : 120,
	"skill_name" : "江河不竭",
	"damage_type":"刺伤"
]),
([
	"action":"$N左手剑鞘一举，快逾电光石光，一招「高塔挂云」，用剑鞘套住$n手中兵器，$w直指$n的咽喉",
	"lvl" : 140,
	"skill_name" : "高塔挂云",
	"damage_type":"刺伤"
]),
([
	"action":"$N翻身回剑，剑诀斜引，一招「百丈飞瀑」，剑锋从半空中直泻下来，罩住$n上方",
	"lvl" : 160,
	"skill_name" : "百丈飞瀑",
	"damage_type":"刺伤"
]),
([
	"action":"$N一式「雪拥蓝桥」，$N手中剑花团团，一条白练疾风般向卷向$n",
	"lvl" : 180,
	"skill_name" : "雪拥蓝桥",
	"damage_type":"刺伤"
]),
([
	"action":"$N腾空而起，突然使出一招「无声无色」，悄无声息地疾向$n的背部刺去",
	"lvl" : 200,
	"skill_name" : "无声无色",
	"damage_type":"刺伤"
]),
});

int valid_enable(string usage) { return (usage == "sword") || (usage == "parry"); }

int valid_learn(object me)
{
	object weapon;
	if (!objectp(weapon = me->query_temp("weapon"))
	 || (string)weapon->query("skill_type") != "sword")
		return notify_fail("你使用的武器不对。\n");

	if ( me->query("max_neili") < 800)
		return notify_fail("你的内力不够。\n");

	if ( me->query_skill("xuantian-wuji", 1) < 100)
		return notify_fail("你的玄天无极功火候太浅。\n");

	if ( me->query("family/master_name") != "何足道" )
		return notify_fail("你只有在拜了何足道后才能学习正两仪剑法。\n");

	if ( me->query_skill("xunlei-jian", 1) < 80)
		return notify_fail("你的迅雷十六剑火候太浅。\n");
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

	level = (int) me->query_skill("liangyi-jian",1);

	for (i = sizeof(action); i > 0; i--) {
		if (level >= action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			if ( me->query_temp("kl/xunlei")){
				return ([
					"action": action[j]["action"],
					"lvl": action[j]["lvl"],
					"damage_type": action[j]["damage_type"],
					"force": 250 +random(350),
					"dodge": random(30),
					"parry": random(30),
					"damage": 130 +random(140),
				]);
			}
			else if( me->query_temp("ly_hebi") ) {
				return ([
					"action": HIC+replace_string(replace_string(replace_string(action[j]["action"], "$w", "$W"), "$W", "$w"HIC), NOR, HIC)+NOR,
					"lvl": action[j]["lvl"],
					"damage_type": action[j]["damage_type"],
					"force": 200 +random(350),
					"dodge": random(30)-10,
					"parry": random(20),
					"damage": 80 +random(140),
				]);
			}
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
				"force": 200 +random(350),
				"dodge": random(30)-10,
				"parry": random(20),
				"damage": 80 +random(140),
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("liangyi-jian", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练两仪剑法。\n");
	if ( me->query("jingli") < 50)
		return notify_fail("你的体力不够练两仪剑法。\n");
	me->receive_damage("jingli", 40);
	me->add("neili",  -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"kl/liangyi-jian/" + action;
}

mixed hit_ob(object me, object victim, int damage_bonus, int factor)
{
	int i;

	i = me->query_skill("liangyi-jian",1)/4;

	if( me->query_temp("ly_hebi")
	 && !me->query_temp("hebi_hit") 
	 && random(10) > 5
	 && me->query_skill_mapped("parry") == "liangyi-jian") {
		me->set_temp("hebi_hit", 1);
		me->add_temp("apply/attack", i);
		COMBAT_D->do_attack(me, victim, me->query_temp("weapon"), 1);  
		me->add_temp("apply/attack", -i);
		me->delete_temp("hebi_hit");
	}
}
