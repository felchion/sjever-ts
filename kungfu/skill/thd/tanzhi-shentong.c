#include <ansi.h>
#include <skill_class.h>
inherit SKILL;

string query_class() {return "险";}

string *xue_name1 = ({ 
        "大锥穴",
        "百会穴",
        "志室穴",
        "肩井穴",
        "劳宫穴",
        "内关穴",
        "外关穴",
        "会宗穴",
});
string *xue_name2 = ({
        "曲池穴",
        "冲阳穴",
        "臂儒穴",
        "白海穴",
        "灵台穴",
        "风府穴",
        "神风穴",
        "玉书穴",
}); 
string *xue_name3 = ({
        "青门穴",
        "商阳穴",
        "丝空竹穴",
        "气海穴",
        "冲门穴",
        "大椎穴",
        "百里穴",
        "槐阳穴",
}); 

mapping *action_finger = ({
([
	"action" : "$N中指微屈突然弹出，一式「指点江山」挟着一缕劲风直袭$n的$l",
	"lvl" : 0,
	"skill_name" : "指点江山",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N左掌在身前一划，右手突然穿出，中指微弹，「如来神指」直袭$n的$l",
	"lvl" : 60,
	"skill_name" : "如来神指",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N右臂挥动中指连弹，在身前划了一道弧线，正是「一衣带水」划向$n的前胸",
	"lvl" : 120,
	"skill_name" : "一衣带水",
	"damage_type" : "割伤"
]),
([
	"action" : "$N跃起在半空，双手急挥中指连弹，一式「弹指惊雷」挟着雷鸣之声铺天盖地袭向$n",
	"lvl" : 180,
	"skill_name" : "弹指惊雷",
	"damage_type" : "刺伤"
]),
});

mapping *action_throwing = ({
([
	"action" : "$N中指微屈突然弹出，一式「指点江山」，$w破空之声甚厉，直射$n的$l",
	"lvl" : 0,
	"skill_name" : "指点江山",
	"damage_type" : "刺伤",
	"post_action": (:call_other, WEAPON_D, "throw_weapon":)
]),
([
	"action" : "$N左掌在身前一划，右手突然穿出，中指微弹，「如来神指」！$w呼啸着直射$n的$l",
	"lvl" : 60,
	"skill_name" : "如来神指",
	"damage_type" : "刺伤",
	"post_action": (:call_other, WEAPON_D, "throw_weapon":)
]),
([
	"action" : "$N右臂挥动中指连弹，数个$w在身前划了一道弧线，正是「一衣带水」，射向$n的前胸",
	"lvl" : 120,
	"skill_name" : "一衣带水",
	"damage_type" : "刺伤",
	"post_action": (:call_other, WEAPON_D, "throw_weapon":)
]),
([
	"action" : "$N跃起在半空，双手急挥中指连弹，「弹指惊雷」弹出铺天盖地的$w，向$n急射而去",
	"lvl" : 180,
	"skill_name" : "弹指惊雷",
	"damage_type" : "刺伤",
	"post_action": (:call_other, WEAPON_D, "throw_weapon":)
]),
});

int valid_enable(string usage) { return usage=="finger" || usage=="throwing" || usage=="parry"; }

int valid_learn(object me)
{
	object weapon;
	if (weapon = me->query_temp("weapon"))
	if ((string)weapon->query("skill_type") != "throwing")
		return notify_fail("学弹指神通必须空手或手持暗器。\n");
	if ( me->query_skill("bihai-chaosheng", 1) < 100)
		return notify_fail("你的碧海潮生功火候不够，无法学弹指神通。\n");
	if (me->query_skill_mapped("force") != "bihai-chaosheng")
		return notify_fail("修习弹指神通必须有碧海潮生功配合。\n");
	if ( me->query("max_neili") < 200)
		return notify_fail("你的内力太弱，无法练弹指神通。\n");
	return 1;
}

string query_skill_name(int level)
{
	int i;
	for(i = sizeof(action_finger)-1; i >= 0; i--)
		if(level >= action_finger[i]["lvl"])
			return action_finger[i]["skill_name"];
}


mapping query_action(object me, object weapon)
{
	int i, level, j,bonus=0;
	level   = (int) me->query_skill("tanzhi-shentong",1);
	if (me->query("gender")=="女性") bonus=20;
	if ( level > 250
	&& me->query_skill("force") > 250
	&& me->query("neili") >= 200
	&& 40-bonus > random(320)
	&& !me->query("thd/huayu")) {
		me->add("neili", -20);
		if (!weapon)
		return ([
			"action": HIW"$N一声断喝，身形开始急速晃动，绕着$n游走。$n一错愕间，只听“嗤嗤”之声连响，无数道劲风从四面八方袭向$n" NOR,
			"damage_type": "刺伤",
			"skill_class" 	: XIAN,
			"skill_damage" 	: 30-bonus,
			"skill_attack" 	: 30+bonus,
			"skill_parry" 	: -40,
			"skill_dodge"		: bonus,
			"skill_kill" 		: 10-bonus,
			"skill_rush" 		: 50-bonus,
			"skill_hurt" 	  : 20,
			"skill_GP" : 3
		]);
		else
		return ([
			"action": HIR"$N身形开始急速晃动，绕着$n游走。$n一错愕间，只听“嗤嗤”之声连响，无数" + weapon->query("unit") + weapon->query("name") + HIR"从四面八方射向$n" NOR,
			"damage_type": "刺伤",
			"skill_class" 	: XIAN,
			"skill_damage" 	: 40-bonus,
			"skill_attack" 	: 30+bonus,
			"skill_parry" 	: -40,
			"skill_dodge"		: bonus,
			"skill_kill" 		: 50-bonus,
			"skill_rush" 		: -bonus,
			"skill_hurt" 	  : 30,
			"skill_GP" : 3
		]);
	}
	if (!weapon) {
		for(i = sizeof(action_finger); i > 0; i--) {
			if(level >= action_finger[i-1]["lvl"]) {
				j = NewRandom(i, 20, level/5);
				return ([
					"action": action_finger[j]["action"],
					"lvl": action_finger[j]["lvl"],
					"damage_type": action_finger[j]["damage_type"],
					"skill_class" 	: XIAN,
					"skill_damage" 	: 20-bonus,
					"skill_attack" 	: 20+bonus,
					"skill_parry" 	: 20,
					"skill_dodge"		: bonus,
					"skill_kill" 		: 20-bonus,
					"skill_rush" 		: 0,
					"skill_hurt" 	  : 10-bonus,
					"skill_GP" 			: 2
				]);
			}
		}
	}
	else {
		for(i = sizeof(action_throwing); i > 0; i--) {
			if(level > action_throwing[i-1]["lvl"]) {
				j = NewRandom(i, 20, level/5);
				return ([
					"action": action_throwing[j]["action"],
					"lvl": action_throwing[j]["lvl"],
					"damage_type": action_throwing[j]["damage_type"],
					"post_action": action_throwing[j]["post_action"],
					"skill_class" 	: XIAN,
					"skill_damage" 	: 20-bonus,
					"skill_attack" 	: 20+bonus,
					"skill_parry" 	: -20,
					"skill_dodge"		: bonus,
					"skill_kill" 		: 0,
					"skill_rush" 		: 20-bonus,
					"skill_hurt" 	  : 10-bonus,
					"skill_GP" 			: 2
				]);
			}
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("tanzhi-shentong", 1);
	int i = sizeof(action_finger);
	mapping learned = me->query_learned();

	if (!mapp(learned)) learned = ([]);
	if (!learned["tanzhi-shentong"])
		while (i--) 
		if (lvl == action_finger[i]["lvl"])
			return notify_fail("你将已经学会的所有招式练习了一遍，发现如果不去找师父学新的招式就无法获得进步了。\n");;
	if ( me->query("jingli") < 50)
		return notify_fail("你的体力太低了。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练弹指神通。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"thd/tanzhi-shentong/" + action;
}

mixed hit_ob(object me, object victim, int damage_bonus, int factor)
{
	int lvl = me->query_skill("finger");
	string name1,name2, name3;

	if ( me->query_temp("thd/feiying") ) return 0;

	if ( lvl > 200 
	 && me->query("neili") > 600
	 && !victim->is_busy()
	 && !random(5)
	 && !me->query_temp("weapon")){
		name1 = xue_name1[random(sizeof(xue_name1))];
		name2 = xue_name2[random(sizeof(xue_name2))];
		name3 = xue_name3[random(sizeof(xue_name3))];
		message_vision(HIR "$n被$N点中了「"HIC + name1 + HIR"」，内息大乱！\n" NOR,me,victim);
		victim->add_busy(random(3)+1);
		if (random(lvl) > 230) {
			message_vision(HIY "$N手指微动，又点中了$n的「"HIC + name2 + HIY"」！\n" NOR,me,victim);
			message_vision(HIY "$n只觉得头微微晕眩，精神不能集中！\n" NOR,me,victim);
			victim->add("jingli",- lvl); 
		}
		if (random(lvl) > 300) {
			message_vision(HIG "$N反手再点向$n的「"HIC + name3 + HIG"」！\n" NOR,me,victim);
			message_vision(HIG "$n丹田一阵剧痛，内力无法凝聚！\n" NOR,me,victim);
			victim->add("neili", - lvl*2); 
		}
	}
}
