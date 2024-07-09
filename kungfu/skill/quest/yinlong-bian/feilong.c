#include <ansi.h>
inherit F_SSERVER;
void attack_1(object me);
void attack_2(object me);
void attack_3(object me);
void finish(object me);

int query_ap() {return 3;}

int lv;
int perform(object me, object target)
{
	string msg;
	object weapon = me->query_temp("weapon");

	if( !target ) target = offensive_target(me);

	if( !me->query("quest/jiuyin2/pass") && !me->query("quest/jiuyin1/pass") )
	return notify_fail("你无法使用「飞龙」。\n");

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("［飞龙］只能对战斗中的对手使用。\n");


	if( lv=me->query_skill("yinlong-bian",1) < 180
	|| me->query_skill("jiuyin-zhengong",1) < 180 )
	return  notify_fail("你的功夫还不够娴熟，不会使用飞龙。\n");

	if( !weapon
	|| weapon->query("skill_type") != "whip"
	|| me->query_skill_mapped("whip") != "yinlong-bian"
	|| me->query_skill_mapped("parry") != "yinlong-bian")
	return  notify_fail("你现在无法使用飞龙。\n");

	if( me->query("neili") < 2000 )
	return notify_fail("你的内力不够。\n");

	if( me->query("jingli") < 1500 )
	return notify_fail("你的精力不够。\n");


	msg = HIG"\n$N使出九阴银龙鞭的「飞龙」，将一条鞭子使的滴水不漏。\n"NOR;

	me->add("apply/pfm_dodge",300);
	// busy 在各个 callout 连续增加
	me->start_busy(random(2));
	message_vision(msg, me, target);
	call_out("attack_1",3);
	me->add("jingli", -200);
	me->add("neili", -300);
	return 1;
}

void attack_1(object me)
{
	object target;
	if(!me) return;
	target = offensive_target(me);
	if(!target)
	{
		call_out("finish",1,me);
		return;
	}

	if (me->is_fighting()) {
		me->add_temp("apply/pfm_attack", 90+lv/10);
		me->add_temp("apply/damage", 200+lv/5);
		me->add_temp("apply/pfm_damage", 200+lv/5);
		message_vision(HIY"$N突然抽空挥鞭向$n攻去！\n"NOR, me, target);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);
		me->add_temp("apply/pfm_attack", -90-lv/10);
		me->add_temp("apply/damage", -200-lv/5);
		me->add_temp("apply/pfm_damage", -200-lv/5);
		call_out("attack_2",3,me);
	}
	else
	call_out("finish",1,me);
}

void attack_2(object me)
{
	object target;
	if(!me) return;
	target = offensive_target(me);
	if(!target)
	{
		call_out("finish",1,me);
		return;
	}
	if (me->is_fighting()) {
		me->add_temp("apply/pfm_attack", 90+lv/10);
		me->add_temp("apply/damage", 200+lv/5);
		me->add_temp("apply/pfm_damage", 200+lv/5);
		message_vision(HIY"$N突然抽空挥鞭向$n攻去！\n"NOR, me, target);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);
		me->add_temp("apply/pfm_attack", -90-lv/10);
		me->add_temp("apply/damage", -200-lv/5);
		me->add_temp("apply/pfm_damage", -200-lv/5);
		call_out("attack_3",3,me);
	}
	else
	call_out("finish",1,me);
}

void attack_3(object me)
{
	object target;
	if(!me) return;
	target = offensive_target(me);
	if(!target)
	{
		call_out("finish",1,me);
		return;
	}
	if (me->is_fighting()) {
		me->add_temp("apply/pfm_attack", 90+lv/10);
		me->add_temp("apply/damage", 200+lv/5);
		me->add_temp("apply/pfm_damage", 200+lv/5);
		message_vision(HIY"$N突然抽空挥鞭向$n攻去！\n"NOR, me, target);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);
		me->add_temp("apply/pfm_attack", -90-lv/10);
		me->add_temp("apply/damage", -200-lv/5);
		me->add_temp("apply/pfm_damage", -200-lv/5);
		call_out("finish",1,me);
	}
	else
	call_out("finish",1,me);
}

void finish(object me)
{
	if(!me) return;
	message_vision(HIY"$N力气不济，鞭法缓了下来。\n"NOR, me);
	me->add("apply/pfm_dodge",-300);
}

string perform_name(){ return HIG"飞龙诀"NOR; }

int help(object me)
{
        write(HIG"\n九阴银龙鞭之「飞龙诀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 2000 以上；
                最大内力 1500 以上；
                当前精力 1500 以上；
                九阴银龙鞭等级 180 以上；
                九阴真功等级 180 以上；
                激发鞭法为九阴银龙鞭；
                激发招架为九阴银龙鞭；
                且手持兵器。

HELP
        );
        return 1;
}
