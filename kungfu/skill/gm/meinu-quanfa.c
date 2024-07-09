// meinu-quan 美女拳法
// Modify By River
// Modified by darken@SJ

#include <ansi.h>;
inherit SKILL;
#include <skill_class.h>

string query_class() {return "妙";}

mapping *action = ({
([
	"action" : "$N使一招「貂禅拜月」，右手一挥，向$n的$l绕去，掌缘在$n的$l一斩",
	"skill_name" : "貂禅拜月",
	"lvl" : 0,
	"damage_type" : "内伤"
]),
([
	"action" : "$N双拳抱胸，忽地右手使一招「西施捧心」，向$n的$l插去",
	"skill_name" : "西施捧心",
	"lvl" : 10,
	"damage_type" : "震伤"
]),
([
	"action" : "$N右手五指弹起，仿似弹习琵琶一般，一招「昭君出塞」五指轮番向$n弹去", 
	"skill_name" : "昭君出塞",
	"lvl" : 20,
	"damage_type" : "震伤"
]),
([
	"action" : "$N侧身一闪，双手合拳向上抬去，一招「麻姑献寿」击向$n的$l",
	"skill_name" : "麻姑献寿",
	"lvl" : 30,
	"damage_type" : "内伤"
]),
([
	"action" : "$N施出「天女织梭」，右手挥左，做投梭织布之状，一挥一送，击向$n的$l",
	"skill_name" : "天女织梭",
	"lvl" : 50,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N施出「则天垂帘」，身子前扑，双掌以垂帘之势削将下来，斩向$n的$l",
	"skill_name" : "则天垂帘",
	"lvl" : 70,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N脸露微笑，伸手往头上一梳，手指如「丽华梳妆」，软软挥将出去，拍向$n的$l",
	"skill_name" : "丽华梳妆",
	"lvl" : 90,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N双手互拍，闪电般击出，俨然一招「红玉击鼓」，轮番击向$n$l",
	"skill_name" : "红玉击鼓",
	"lvl" : 110,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N一招「弄玉吹箫」，一指擎天，一指对地，闭目垂首，忽手腿齐出，攻向$n的$l",
	"skill_name" : "弄玉吹箫",
	"lvl" : 120,
	"damage_type" : "瘀伤"
]),
});

int valid_enable(string usage) { return usage=="cuff" || usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("空手方能练习美女拳法。\n");
	if ( me->query_skill("yunu-xinjing", 1) < 10)
		return notify_fail("玉女心经领悟不够，无法修习美女拳法。\n");
	if ( me->query_skill("dodge", 1) < 20)
		return notify_fail("你的轻功太差，无法练美女拳法。\n");
	return 1;
}

string query_skill_name(int level)
{
	int i;
	for(i = sizeof(action)-1; i >= 0; i--)
		if(level >= action[i]["lvl"])
			return action[i]["skill_name"];
}

mapping query_action(object me, object weapon,object ob)
{
	int i, j, level;

	level = (int) me->query_skill("meinu-quanfa",1);         

	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action" : action[j]["action"],
				"force" : 250+random(300),
				"lvl" : action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"skill_class"   : MIAO,
        "skill_damage"  : 0,
        "skill_attack"  : 30,
        "skill_parry"   : -20,                           
        "skill_dodge"   : 10,
        "skill_kill"    : 0,
        "skill_rush"    : 30,
        "skill_hurt"    : 0,
        "skill_GP"      : 2, 
			]);
		}
	}
}

string perform_action_file(string action)
{
	return __DIR__"gm/meinu-quanfa/" + action;
}

int practice_skill(object me)
{
	if (me->query("jingli") < 40)
		return notify_fail("你的体力太低了。\n");
	if (me->query("neili") < 20)
		return notify_fail("你的内力不够练美女拳法了。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}


int help(object me)
{
        write(WHT"\n美女拳法："NOR"\n\n");
        write(@HELP
	武功名称：
	        美女拳法(meinu-quanfa)
             
	特殊攻击：   
	        摄心绝技(perform shexin)
	        百媚横生(perform baimei)
        
HELP
        );
        return 1;
}
