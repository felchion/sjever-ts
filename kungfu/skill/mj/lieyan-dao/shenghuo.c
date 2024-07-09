// guangming.c  圣火光明
// By River@SJ

#include <ansi.h>
inherit F_SSERVER;

string perform_name(){ return HIC"圣火光明"NOR; }

int query_ap() {return 3;}

string *xue_name = ({ 
"劳宫穴","膻中穴","曲池穴","关元穴","曲骨穴","中极穴","承浆穴","天突穴","百会穴",
"幽门穴","章门穴","大横穴","紫宫穴","冷渊穴","天井穴","极泉穴","清灵穴","至阳穴",
}); 

string *limb=({
"颈部","后心","右腿","头部","小腹","胸口","腰间","裆部","右肩","右臂","右脚",
});

int perform(object me, object target)
{
	object weapon;
	int lvl;
	string msg,name,dodge_skill,limbs;

	if(!target) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("「圣火光明」只能对战斗中的对手使用。\n");

	if (!objectp(weapon = me->query_temp("weapon"))
	||(string)weapon->query("skill_type") != "blade")
	return notify_fail("你使用的武器不对。\n");

	if((lvl=(int)me->query_skill("lieyan-dao", 1)) < 100 )
	return notify_fail("你的烈焰刀法不够娴熟，不会使用「圣火光明」。\n");

	if ( !me->query_skill("jiuyang-shengong", 1))
	if ( me->query_skill("shenghuo-shengong", 1) < 100 )
	return notify_fail("你的圣火神功不够高，不能使用「圣火光明」。\n");

	if((int)me->query_skill("piaoyi-shenfa", 1) < 100 )
	return notify_fail("你的飘翼身法不够高，不能使用「圣火光明」。\n");

	if( me->query_temp("qkdny/yin") )
	return notify_fail("你正在施展乾坤大挪移「引」字诀呢！\n");

	if( me->query_temp("qkdny/hua") )
	return notify_fail("你正在施展乾坤大挪移「化」字诀呢！\n");

	if( me->query_skill_mapped("blade") != "lieyan-dao")
	return notify_fail("你现在的状态无法使用「圣火光明」。\n");

	if ((int)me->query("neili",1) < 1000)
	return notify_fail("你的真气不够，无法使用「圣火光明」。\n");

	limbs= limb[random(sizeof(limb))];
	me->set_temp("apply/action/pfm", HIC"$N大吼一声："HIR"为善除恶，惟光明故，喜乐悲愁，皆归尘土"HIC"。"+weapon->query("name")+HIC"和人合二为一，带着漫天杀气扑向$n"NOR);
	me->set_temp("apply/action/hit", CYN"$n只觉得所有退路都被这逼人的杀气笼罩，避无可避，不由大惊失色，登时眼前一花，"+weapon->name()+CYN"已从其"+limbs+"洞穿而出！\n"NOR);


	me->receive_damage("neili", 500+lvl/2);
	me->add_temp("apply/damage",600+lvl);
	me->add_temp("apply/pfm_damage",200+lvl/5);
	me->add_temp("apply/pfm_attack",150+lvl/10);
	COMBAT_D->do_attack(me, target,weapon,3);
	me->add_temp("apply/damage",-600-lvl);
	me->add_temp("apply/pfm_damage",-200-lvl/5);
	me->add_temp("apply/pfm_attack",-150-lvl/10);
	me->delete_temp("apply/action/");

	if( me->query_skill("yingzhua-shou",1 ) > 100
	&& me->query_skill_mapped("hand") == "yingzhua-shou"
	&& me->query_skill_prepared("hand") == "yingzhua-shou"
	&& ! target->is_busy()){
		name = xue_name[random(sizeof(xue_name))];
		msg = HIC"\n$N紧跟着上前一步，乘对方不备之际，左手变掌为鹰爪之势，自左向右抓向$n的"+name+"。\n"NOR;

		if( COMBAT_D->do_attack(me, target, weapon, 0, 1) || COMBAT_D->do_attack(me, target, weapon, 0, 1)) {
			target->add_busy(5);
			msg += CYN"结果$n的"+name+"被抓个正着，全身不由自主的一颤，顿时不可动弹。\n"NOR;
		}
		else {
			dodge_skill = target->query_skill_mapped("dodge");
			if( !dodge_skill ) dodge_skill = "dodge";
			msg += SKILL_D(dodge_skill)->query_dodge_msg(target, 1);
			me->add("neili", -50);
		}
		message_vision(msg, me, target);		
	}
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
        write(HIC"\n烈焰刀法「圣火光明」："NOR"\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                当前精力 300 以上；
                烈焰刀法等级 100 以上；
                飘翼身法等级 100 以上；
                圣火神功等级 100 以上；
                激发刀法为烈焰刀法。

        另外：
                鹰爪擒拿手等级 100 以上；
                激发手法为鹰爪擒拿手；
                备鹰爪擒拿手；
                有附加作用。

HELP
        );
        return 1;
}
