// modified by snowman@SJ 05/12/2000
// By Spiderii参照修改。。图省事。。grin
inherit F_SSERVER;

#include <ansi.h>
#include <combat.h>
#include <combat_msg.h>

int query_ap() {return 10;}

int perform(object me,object target)
{
	object weapon;
	string msg;
	int lvl;

	lvl = me->query_skill("huifeng-jian",1);

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) || !living(target)
	|| environment(target)!=environment(me))
	return notify_fail("灭绝心法只能对战斗中的对手使用。\n");

	if(me->query_skill_mapped("force") != "linji-zhuang")
	return notify_fail("你的内功不是峨嵋心法，无法使用灭绝心法！\n");
	if( me->query_skill("linji-zhuang", 1) < 150 )
	return notify_fail("你的内功还未练成，不能使用灭绝心法！\n");
	if( me->query_skill("huifeng-jian", 1) < 150 )
	return notify_fail("你的剑法还未练成，不能使用灭绝心法！\n");
	if (!objectp(weapon = me->query_temp("weapon")) || weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "huifeng-jian")
	return notify_fail("你手里没有剑，无法使用灭绝心法！\n");

	if(me->query_skill_mapped("parry") != "huifeng-jian")
	return notify_fail("你的招架功夫不对，无法使用灭绝心法！\n");
	if(me->query("max_neili") < 2000 )
	return notify_fail("你的内力修为不够，不能使用灭绝心法！\n");
	if(me->query("neili") < 1000 )
	return notify_fail("你现在真气不足，不能使用灭绝心法！\n");
	if(me->query("jingli") < 750 )
	return notify_fail("你现在精力不足，不能使用灭绝心法！\n");
	if(me->query_temp("mie_jue"))
	return notify_fail("你正在使用灭绝心法！\n");
	if(me->query_skill("sword", 1) < 150 )
	return notify_fail("你的基本剑法不够娴熟，不能在剑招中使用灭绝心法。\n");

	if( target->is_busy() )
	return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧。\n");

	msg = HIW "$N飘身而起，使出峨嵋绝技「灭剑」心法，顿时剑意空灵飘逸而，似万千柳絮飞舞，又如和风阵阵拂过。\n"NOR;

	if(COMBAT_D->do_attack(me, target, weapon,0, 1)
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)) {
		msg = msg + HIC "结果$n大吃一惊，慌乱之下破绽迭出，被$N攻了个措手不及。\n" NOR;
		target->add_busy(random(20) + 30);
		me->add_temp("apply/pfm_attack", 240+lvl/10);
		me->add_temp("apply/pfm_damage", 200+lvl/4);
		me->add_temp("apply/damage",  800+lvl);
		me->set_temp("apply/action/color",HIG);
		me->set_temp("mie_jue", 1);
		me->add("neili",-200);
		call_out("next1", 8, me, target, lvl);
	}
	else
	msg = msg + MAG "可是$n冷静非凡，丝毫不为这奇幻的招数所动，凝神抵挡，不漏半点破绽。\n" NOR;

	message_vision(msg, me, target);
	return 1;
}

int next1(object me, object target, int lvl)
{
	string msg;
	object weapon;

	if (!me) return 0;

	weapon = me->query_temp("weapon");
	if (!weapon || !target || !living(me) || !me->is_fighting(target)) {
		me->add_temp("apply/pfm_attack", -240-lvl/10);
		me->add_temp("apply/pfm_damage", -200-lvl/4);
		me->add_temp("apply/damage",  -800-lvl);
		me->delete_temp("apply/action");
		me->delete_temp("mie_jue");
		return 0;
	}

	msg = HIR "$N深吸一口气，运起峨嵋临济十二庄,配合「灭剑」,使出「绝剑」心法！\n"NOR;

	if(COMBAT_D->do_attack(me, target, weapon,0, 1)
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)){
		msg = msg + CYN"$n只觉$N灭剑、绝剑配合异常绝妙，不由压力大增。\n"NOR;
		message_vision(msg, me, target);
		target->add_condition("no_perform",10+random(2));
		target->set_temp("must_be_hit",1);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);
		if (target && objectp(target)) target->delete_temp("must_be_hit");
	}
	else {
		msg  =  msg + CYN"$n临危不乱，脚下步法连变，闪躲开来。\n"NOR;
		message_vision(msg, me, target);
		me->add_busy(1+random(2));
	}
	me->add_temp("apply/pfm_attack", -240-lvl/10);
	me->add_temp("apply/pfm_damage", -200-lvl/4);
	me->add_temp("apply/damage",  -800-lvl);
	me->delete_temp("apply/action");
	me->delete_temp("mie_jue");
	return 1;
}

string perform_name(){ return HIW"灭绝心法"NOR; }

int help(object me)
{
        write(HIW"\n回风拂柳剑之「灭绝心法」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 2000 以上；
                当前精力 750 以上；
                回风拂柳剑等级 150 以上；
                临济十二庄等级 150 以上；
                激发内功为临济十二庄；
                激发剑法为回风拂柳剑；
                激发招架为回风拂柳剑；
                手持兵器。

HELP
        );
        return 1;
}


