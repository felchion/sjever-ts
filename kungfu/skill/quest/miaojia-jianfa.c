// hujia-daofa.c
// query("quest/雪山飞狐/武功/miaojiajianfa")是学习的基础条?
// query("quest/雪山飞狐/武功/ronghe") 是刀剑融合的标志
// query_temp("quest/雪山飞狐/武功/lengquan") 是冷泉内功yun lengquan之后的标志

#include <ansi.h>
#include <skill_class.h>
inherit SKILL;

string query_class() {return "妙";}

mapping *action = ({
	([   "action" : "$N面露微笑，手中$w一抖，回身侧步，迅疾无比，剑光暴长，手中的$w挽了一个剑花洒向$n",
		"lvl"    : 0,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N手中$w颤动的极快，只幌得$n眼花撩乱，浑不知从何攻来，$w从一个绝想不到的方位斜刺向$n",
		"lvl"    : 10,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N暴退数尺，低首抚剑，身形突闪，剑招陡变，左撤步，手中$w骤然穿上，一闪而上刺向$n",
		"lvl"    : 20,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N却身提步，手中的$w似挡非挡，身形一晃，疾掠而上，手中$w龙吟一声，对准$n的$l连递数剑",
		"lvl"    : 30,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N一抖手中的$w，挽出数个剑花，笼罩了$n，剑锋往来运转如梭，手中$w“铮”然有声，往$n刺去",
		"lvl"    : 40,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N碎步急进，提剑沿剑身方向疾速上崩，手中$w由内自外一刺，没有任何花巧，然而快得不可思议",
		"lvl"    : 50,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N身体左转，左手剑指，微微一个转身，手中$w幻化作满天星点，手中$w却已自肋下穿出刺向$n",
		"lvl"    : 60,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N神色微变，平剑斜洗，臂圆剑直，一招「丹凤朝阳」，剑招顿时变得凌厉无比，如匹链般洒向$n",
		"lvl"    : 70,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N左手剑指缓缓划转，腰部一扭，一招「洗剑怀中抱月」，手中$w中宫直进，迅捷无比往$n刺去",
		"lvl"    : 80,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N纵步上走，单腿踢向$n腰间，$n正要抽身回防，只见刹那间剑光一闪，一招「迎门腿反劈华山」",
		"lvl"    : 90,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N矮身侧步，一招「返腕翼德闯帐」，手中$w反手疾挑而出，手中$w轻轻颤动自上而下扎向$n",
		"lvl"    : 100,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N屈腕抬臂，一招「黄龙转身吐须」，蓦地疾退一步，又冲前三步，一道凌厉的弧光往$n刺去",
		"lvl"    : 110,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N一招「上步云边摘月」，纵身跃起，接着却又从半空中穿下，双手握着$w，声势逼人地刺向$n",
		"lvl"    : 120,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N一招「提撩剑白鹤舒翅」，手中$w“嗤”的一声斜刺向天空，随即向下“唰唰”划出一个叉字",
		"lvl"    : 130,
		"damage_type":  "刺伤"
	]),
	([   "action" : "$N屈腕抬臂，左腿微抬，一招「冲天掌苏秦背剑」，右手$w猛的自下方挑起，激起一股劲风反挑$n",
		"lvl"    : 140,
		"damage_type":  "刺伤"
	]),
});

int valid_enable(string usage) {
	if ( this_player()->query("quest/雪山飞狐/武功/miaojiajianfa"))
	return usage == "sword" || usage == "parry";
}

int valid_learn(object me)
{
	if ((int)me->query("max_neili") < 700)
	return notify_fail("你的内力不够。\n");

	if ((int)me->query_skill("force") < 90)
	return notify_fail("你的内功火候太浅。\n");

	if ((int)me->query_skill("sword", 1) < 100)
	return notify_fail("你的基本剑法火候不够，无法学习苗家剑法。\n");

	if ((int)me->query_skill("sword", 1) < (int)me->query_skill("miaojia-jianfa", 1))
	return notify_fail("你的基本剑法水平有限，无法领会更高深的苗家剑法。\n");

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
	level   = (int) me->query_skill("miaojia-jianfa",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level >= action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action"				:	action[j]["action"],
				"lvl"						:	action[j]["lvl"],
				"damage_type"		:	action[j]["damage_type"],
				"skill_class" 	: MIAO,
				"skill_damage" 	: 30,
				"skill_attack" 	: 30,
				"skill_parry" 	: 0,
				"skill_dodge"		: 0,
				"skill_kill" 		: 10,
				"skill_rush" 		: 10,
				"skill_hurt" 	  : 100,			
				"skill_GP" 			: 2		
			]);
		}
	}
}


int practice_skill(object me)
{
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon")) ||
	(string)weapon->query("skill_type") != "sword")
	return notify_fail("你使用的武器不对。\n");

	if ((int)me->query("qi") < 60)
	return notify_fail("你的体力目前没有办法练习苗家剑法。\n");

	if ((int)me->query("neili") < 70)
	return notify_fail("你的内力不够，无法练习苗家剑法。\n");

	me->add("qi", -50);
	me->add("neili", -62);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"quest/miaojia-jianfa/" + action;
}



