// shenghuo-lingfa 圣火令法
// 1997-5-14
inherit SKILL;
#include <ansi.h>
#include <skill_class.h>

string query_class() {return "奇";}

string  *msgl = ({
"$N一式「以意相求」忽地放手，$w的尾端向上弹起，以意想不到方位呼的向$n拍去",
"$N平平一令击出，一式「风回路转」，$w在半空古古怪怪的转了个弯，打向$n的$l",
"$N突然一声清啸，双令一招「三虚七实」虚虚实实，是是而非的击向$n的$l",
"$N忽然低头，一式「头头是道」，一个头锤向$n撞去，手中$w却暗中的向$n的$l击去",
"$N突然纵身而起，一式「翻江倒海」连翻三个空心筋斗，白光忽闪，$w向$n的$l砸去",
"$N坐地锤胸，一式「瞒天过海」呼天抢地的哭号，突然间脚踢飞沙，抽出$w向$n疾刺",
"$N默念口诀，$w一招「应左则后」，明指前胸，突地一转弯，向$n的背心大穴砸去",
"$N使出的是圣火令中的一招「须右仍前」，凌空虚点$n右臂，$w却向对方胸前大穴刺去",
"$N一招「无中生有」，不知如何，手中已经多出$w，手腕一摆，快速无比的刺向$n的$l",
"$N手中的$w划出一个个的圈子，这招「天方地园」一出，顿时裹得$n眼花缭乱，不知所措"
});

string  *msgc = ({
"$N反守为攻，一招「乱中求胜」，身体斜飞，连环的向$n踢出四脚",
"$N一式「坐地求全」，忽地在地上一坐，左手抱向$n的$l，右手击向“中都”穴",
"$N突然间一式「以进为退」，接着向前一个空心筋斗，凌空落下，双足向$n$l踢去",
"$N呼的一声，一式「庶天蔽日」，纵身半空翻了几个空心筋斗，伸臂向$n的$l抓去",
"$N一个筋斗，一式「万流归宗」，身在半空，落下时右脚的膝盖在$n关顶一脆",
"$N向前一坐，一式「投怀送抱」，撞入$n的怀中，双肘向$n$l的穴道撞去",
"$N坐地锤胸，一式「飞沙走石」，呼天抢地的哭号，突然间脚踢飞沙，单手向$n疾点"
});



int valid_enable(string usage)
{
        return usage == "dagger" || usage== "cuff" || usage == "parry";
}

mapping query_action(object me, object weapon)
{
	if (!weapon)
		return ([
			"action":msgc[random(sizeof(msgc))],
			"damage_type":"瘀伤",
			"skill_class" 	: QI,
			"skill_damage" 	: 0 ,
			"skill_attack" 	: 10 ,
			"skill_parry" 	: 30 ,
			"skill_dodge"		: 10 ,
			"skill_kill" 		: -50,
			"skill_rush" 		: 50,
			"skill_hurt" 	  : 0,
			"skill_GP" 			: 2
		]);
	return ([
		"action":msgl[random(sizeof(msgl))],
		"damage_type":random(2)?"刺伤":"割伤",
		"skill_class" 	: QI,
		"skill_damage" 	: 0 ,
		"skill_attack" 	: 10 ,
		"skill_parry" 	: 30 ,
		"skill_dodge"		: 10 ,
		"skill_kill" 		: -50,
		"skill_rush" 		: 50,
		"skill_hurt" 	  : 0,
		"skill_GP" 			: 2,
		"post_action": (: call_other, WEAPON_D, "bash_weapon" :)
	]);
}

int practice_skill(object me)
{
	if ( me->query("jingli") < 50)
		return notify_fail("你的体力不够练圣火令法。\n");         
	if ((int)me->query("neili") < 10)
		return notify_fail("你的内力不够练圣火令法。\n");
	me->receive_damage("jingli", 40);
	me->receive_damage("neili", 15);
	return 1;
}

int valid_learn(object me)
{
	object weapon = me->query_temp("weapon");
	int skill = me->query_skill("shenghuo-lingfa", 1);
       
	if (!weapon || weapon->query("skill_type") != "dagger")
		return notify_fail("你没有使用的武器。\n");

	if (me->query("max_neili") < 1000)
		return notify_fail("你的内力不够。\n");
	if (!me->query_skill("jiuyang-shengong"))
	if (me->query_skill("shenghuo-shengong", 1) < (skill-10)
	&& me->query_skill("shenghuo-shengong", 1) < 200)
		return notify_fail("你的圣火神功火候太浅。\n");
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"mj/shenghuo-lingfa/" + action;
}
