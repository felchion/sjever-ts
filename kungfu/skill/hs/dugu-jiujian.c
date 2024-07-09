// dugu-jiujian.c 独孤九剑
// By Spiderii 完善更新 增加450 lv 飞跃效果，完善auto，设定为A级剑法
// Update by lsxk@hsbbs 设定只有剑宗才可以使用，提高效果，有一定几率must_be_hit.  2007/7/26
// 保留lsxk写的描述,取消hit_ob特殊效果 felchion 2008.4.13

inherit SKILL;
#include <ansi.h>
#include <combat.h>
#include <skill_class.h>

string query_class() {return "妙";} 

string *dodge_msg = ({
"$n挺剑一招象是「白云出岫」回刺$N的$l，企图将$N的攻势化解。\n",
"$n不闪不避，举剑闪电般使出「叠翠浮青」反削$N的$l，想挡过$N此招。\n",
"只见$n不退反进，身如飘风，诡异地一式「天柱云气」，动向无定不可捉摸地指向$N的$l。\n",
"$n运剑如风，剑光霍霍中反攻$N的$l，尝试逼$N自守，剑招似是「伏魔剑」的「龙吞式」。\n",
"$n以攻为守，以进为退，凝神运气向$N猛攻快打地挥出方位大异的泰山「快活三」三剑。\n",
"却见$n踏前一步，剑式斜指$N的$l，想要使$N闪身而退。\n",
"$n剑法突变，剑势伸缩不定，奔腾矫夭，逆使嵩山剑法的「天外玉龙」回攻$N。\n",
"$n举剑狂挥，迅速无比地点向$N的$l，却令人看不出其所用是什么招式。\n",
});

string *msg = ({
"$N挺剑回刺$n的$l。此招象是华山剑法的「白云出岫」，嵩山剑法的「万岳朝宗」，逼得$n回守，停止对$N的攻势",
"$N剑随身转，续而刺出十九剑，竟然是华山「玉女十九剑」，但奇的是这十九剑便如一招，手法之快，直是匪夷所思",
"$N奇诡地向$n挥出「泉鸣芙蓉」、「鹤翔紫盖」、「石廪书声」、「天柱云气」及「雁回祝融」衡山五神剑",
"$N突然使出青城派松风剑法的「鸿飞冥冥」，可是方位却有所偏差，$w对着$n一绞，企图突破$n的攻势",
"$N剑招突变，使出衡山的「一剑落九雁」，削向$n的$l，怎知剑到中途，突然转向，大出$n意料之外",
"$N举起$w运使「太极剑」剑意，划出大大小小无数个圆圈，无穷无尽源源不绝地缠向$n",
"却见$N身随剑走，左边一拐，右边一弯，剑招也是越转越加狠辣，竟化「泰山十八盘」为一剑攻向$n",
"$N神色凝重，$w上劈下切左右横扫，挟雷霆万钧之势逼往$n，「伏魔剑」剑意表露无遗",
"$N剑势忽缓而不疏，剑意有余而不尽，化恒山剑法为一剑，向$n慢慢推去",
"$N突然运剑如狂，一手关外的「乱披风剑法」，猛然向$n周身乱刺乱削",
"$N剑意突焕气象森严，便似千军万马奔驰而来，长枪大戟，黄沙千里，尽括嵩山剑势击向$n",
"$N手中$w越转越快，使的居然是衡山的「百变千幻云雾十三式」，剑式有如云卷雾涌，旁观者不由得目为之眩",
"只见$N吐气开声，一招似是「独劈华山」，手中$w向下斩落，直劈向$n的$l",
"但见$N挺身而上，$w一旋，一招仿佛泰山剑法的「来鹤清泉」直刺$n的$l",
"$n窥准$N一招华山剑法「有凤来仪」的趋势，准备进行反击，却见$N突然虚步提腰，使出酷似武当「蜻蜓点水」的一招",
"$n攻向$N的$l，$N不退反进，使出恒山剑招「绵里藏针」，森森剑气充溢四周",
"$N飞身跃起，出剑宛如「金雁横空」，但不等剑招变老，已转为「风声鹤泣」，剑尖划向$n的$l",
"$N剑光一吐，长剑化作一道白虹，向$n直刺过去。这一招端严雄伟，似是嵩山剑法中的「千古人龙」，又似是华山剑法的「无边落木」",
"$N神声凝重，$w上劈下切左右横扫，挟雷霆万钧之势逼往$n，「伏摩剑」剑意表露无遗",
"$N含笑抱剑，气势庄严，$w轻挥，尽融「达摩剑」为一式，闲舒地刺向$n"
});

string *feiyue_msg = ({
"$N突然一剑点向$n的$l，虽一剑却暗藏无数后着，$n手足无措，不知如何是好。",
"$N随手一剑指向$n，落点正是$n的破绽所在，端的是神妙无伦，不可思议。",
"$N将$w随手一摆，但见$n自己向$w撞将上来，神剑之威，实人所难测。",
"$N抱剑旋身，转到$n身后，杂乱无章地向$n刺出一剑，不知使的是什么剑法。",
"$N剑挟刀势，大开大阖地乱砍一通，但招招皆击在$n攻势的破绽，迫得$n不得不守。",
"$N脸上突现笑容，似乎已看破$n的武功招式，胸有成竹地一剑刺向$n的$l。",
"$N满场游走，东刺一剑，西刺一剑，令$n莫明其妙，分不出$N剑法的虚实。",
"$N反手横剑刺向$n的$l，这似有招似无招的一剑，威力却是奇大，$n难以看清剑招来势。",
});


int valid_enable(string usage)
{
   if ( this_player()->query("quest/dgjj/fengset") && this_player()->query("quest/huashan")=="剑宗" )
		return (usage == "sword") || (usage == "parry") || (usage =="dodge");
}

int valid_learn(object me)
{
	mapping my_fam  = me->query("family");
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "sword")
		return notify_fail("你使用的武器不对。\n");

	if(!my_fam || my_fam["master_name"] != "风清扬")
		return notify_fail("你不是风清扬的弟子，不能学习「独孤九剑」。\n");

	if ( me->query("max_neili") < 1000)
		return notify_fail("你的内力不够。\n");

	if ( me->query_skill("dugu-jiujian",1) > 60 && me->query_skill("dugu-jiujian", 1) < 300 )
		return notify_fail("这套剑法我就教到这儿，以后就要靠你自己去领悟了。\n");
	return 1;
}


mapping query_action(object me, object weapon)
{
	int level;

	level = me->query_skill("dugu-jiujian", 1);

		if( level > 500 ) {
			return ([
        "action": HIW+replace_string(replace_string(feiyue_msg[random(sizeof(feiyue_msg))], "$w", "$w"), "$w", "$w"HIW)+NOR,
		    "skill_class"   : MIAO,
		    "skill_damage"  : 0,
		    "skill_attack"  : 20,
		    "skill_parry"   : 30,
		    "skill_dodge"   : 0,
		    "skill_kill"    : 0,
		    "skill_rush"    : 20,
		    "skill_hurt"    : 0,
		    "skill_improve" : 2

			]);
		}
		else return ([
			"action": msg[random(sizeof(msg))],
		  "skill_class"   : MIAO,
		  "skill_damage"  : 0,
		  "skill_attack"  : 20,
		  "skill_parry"   : 30,
		  "skill_dodge"   : 0,
		  "skill_kill"    : 0,
		  "skill_rush"    : 0,
		  "skill_hurt"    : 0,
		  "skill_improve" : 2
		]);
}

mixed hit_ob(object me, object victim, int damage)
{
	victim->add_temp("lost_attack", random(2));
}

string query_dodge_msg(string limb)
{
	return dodge_msg[random(sizeof(dodge_msg))];
}

int practice_skill(object me)
{
	if ( me->query_skill("dugu-jiujian", 1 ) < 300)
		return notify_fail("独孤九剑只能通过领悟来提高。\n");

	if ( me->query("jingli") < 50)
		return notify_fail("你的体力不够练独孤九剑。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练习独孤九剑。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"hs/dugu-jiujian/" + action;
}

