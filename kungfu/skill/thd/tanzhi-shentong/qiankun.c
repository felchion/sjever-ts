// 弹指神通之乾坤一指, pfm qiankun

inherit F_SSERVER;
#include <ansi.h>

string perform_name(){ return HIG"乾坤一指"NOR; }

int query_ap() {return 4;}

int perform(object me, object target)
{
	object weapon;
	string msg;

	if( !target )
	target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("「乾坤一指」只能对战斗中的对手使用。\n");

	if( (int)me->query_skill("bihai-chaosheng", 1) < 400 )
	return notify_fail("你的碧海潮生功修为未到，无法使出乾坤一指。\n");

	//该武功禁止部分npc使用
	//目前主要应用在蝶梦楼npc上
	if( me->query("quest_cancel"))
	return notify_fail("你被禁止使用这门绝技。\n");

	if( (int)me->query_skill("suibo-zhuliu", 1) < 400 )
	return notify_fail("你的随波逐流修为未到，无法使出乾坤一指。\n");

	if( (int)me->query_skill("tanzhi-shentong", 1) < 400 )
	return notify_fail("你的弹指神通修为未到，无法使出乾坤一指。\n");

	if ( me->query_skill_mapped("parry") != "tanzhi-shentong"
	|| me->query_skill_mapped("throwing") != "tanzhi-shentong"
	|| me->query_skill_mapped("finger") != "tanzhi-shentong"
	|| me->query_skill_prepared("finger") != "tanzhi-shentong" )
	return notify_fail("你现在的情况，无法使出乾坤一指。\n");

	weapon = me->query_temp("weapon");

	if ( weapon && weapon->query("skill_type") != "throwing")
	return notify_fail("你装备的武器不对。\n");

	if( (int)me->query_skill("finger",1) < 350 )
	return notify_fail("你的指法修为不足，使不出乾坤一指。\n");

	if( (int)me->query("neili") < 3000 )
	return notify_fail("你现在内力太弱，不能出乾坤一指。\n");

	if (target->query_temp("thd/qiankun"))
	return notify_fail("他已经被乾坤一指击中了，放心攻击吧。\n");

	msg = HIG"\n$N手指一伸，一股劲风随之向$n袭去！\n"NOR;


	if ( COMBAT_D->do_attack(me, target, 0, 0, 1) 
	|| COMBAT_D->do_attack(me, target, 0, 0, 1)
	|| COMBAT_D->do_attack(me, target, 0, 0, 1)){
		msg += HIR"$n被这一指重重点中气海穴，结果檀中、玉堂穴气息再也无法顺畅通行。\n"NOR;
		target->add_busy(random(4)+1);
		target->apply_condition("qiankun_wound",10+random(4));
		me->add("neili",-500);
	}
	else {
		msg += HIY"$n身形拔起，在$N肩头一点，便闪过了这一招。\n"NOR;
	}
  me->start_busy(random(2));
	message_vision(msg, me, target);
	return 1;
}

int help(object me)
{
        write(HIG"\n弹指神通之「乾坤一指」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 3000 以上；
                碧海潮生功 400 级以上；
                随波逐流 400 级以上；
                弹指神通 400 级以上；
                基本指法 350 级以上。
                激发暗器为弹指神通；
                激发招架为弹指神通；
                激发指法为弹指神通；
                备弹指神通；
                空手或拿暗器。

HELP
        );
        return 1;
}

