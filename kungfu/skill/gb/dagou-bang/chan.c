// chan.c 打狗棒法--缠字诀
// Modify by looklove@SJ 3/16/2001
// Update by lsxk@hsbbs 2007/7/2

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

int query_ap() {return 7;}
string perform_name(){ return HBGRN"缠字诀"NOR; }

int perform(object me, object target)
{
	string msg;
	object tweapon, weapon;
	int busy, mlvl;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target) )
		return notify_fail("「缠字诀」只能对战斗中的对手使用。\n");

	weapon = me->query_temp("weapon");

	if( !objectp(weapon)
	 || weapon->query("skill_type") != "stick"
	 || me->query_skill_mapped("stick") != "dagou-bang" )
		return notify_fail("你现在无法使用「缠字诀」。\n");

	mlvl = (int)me->query_skill("dagou-bang",1);


	tweapon = target->query_temp("weapon");

	if( mlvl < 100 )
		return notify_fail("你的打狗棒法还不够娴熟，不会使用「缠字诀」。\n");
	if( me->query_skill("bangjue",1) < 100 )
		return notify_fail("你的打狗棒诀不够娴熟，无法使出「缠字诀」。\n");

	if( (int)me->query_skill("huntian-qigong", 1) < 100 )
		return notify_fail("你的混天气功根基不足，无法使出「缠字诀」。\n");

	if( (int)me->query("neili") < 500 )
		return notify_fail("你的内力不够，无法使出「缠字诀」。\n");

	if( (int)me->query("jingli") < 100 )
		return notify_fail("你的精力不足以让你使出「缠字诀」。\n");

	if( (int)me->query("max_neili") < 1000 )
		return notify_fail("你的内力不够支持你使用「缠字诀」。\n");

	if( target->is_busy()||target->query_temp("dgb/chan") )
		return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧。\n");

  busy = 20;
	if( mlvl > 500 ) busy += (mlvl-500)/50;

	msg = HBGRN"$N信手使出「缠字诀」，手中"+weapon->name()+HBGRN"舞成一团棒影，直往$n罩了过去。\n"NOR;
	if( tweapon )
		msg += HBGRN"只见$n的"+tweapon->name()+HBGRN"挥到东，"+weapon->name()+HBGRN"就跟向东，"+tweapon->name()+HBGRN"打到西，"+weapon->name()+HBGRN"随到西。\n"NOR;
	else
		msg += HBGRN"只见"+weapon->name()+HBGRN"如影随形，招招不离$n周身大穴，当真精妙无比。\n"NOR;

//	message("wizard", HIG"缠字诀：busy回合数"+busy+"，使用者EXP"+mexp+"，对手EXP"+texp+", "+x+","+y+"。\n"NOR, environment(me), me);
	if( COMBAT_D->do_attack(me, target, weapon, 0, 1) || COMBAT_D->do_attack(me, target, weapon, 0, 1) ){
		msg += HIM "$n左蹿右跳，始终逃不出棒影，无论闪向哪里，都有"+weapon->name()+HIM"随后跟到。\n" NOR;
		target->start_busy(busy);
		target->set_temp("dgb/chan", 1);
		call_out("check_fight", 1, me, target, weapon, busy);
	}
	else if( COMBAT_D->do_attack(me, target, weapon, 0, 1) || COMBAT_D->do_attack(me, target, weapon, 0, 1) ){
		msg += HIM "结果$n顿时手忙脚乱，施展出浑身解数，也脱不开$N的棒影。\n" NOR;
		target->start_busy(busy/2);
		target->set_temp("dgb/chan", 1);
		call_out("check_fight", 1, me, target, weapon, busy);
	}
	else {
		msg += HIW "$n镇定自若，寻个破绽，信手在"+weapon->name()+HIW"上一点，漫天棒影顿时消失了。\n" NOR;
		me->start_busy(random(2));
	}
	me->add("neili", -200);
	me->add("jingli", -60);
	message_vision(msg, me, target);
	return 1;
}

void check_fight(object me,object target,object weapon,int busy)
{
	if( ! me
	 || ! target
	 || ! target->query_temp("dgb/chan")
	 || ! me->is_fighting(target)
	 || weapon != me->query_temp("weapon")
	 || me->query_skill_mapped("stick") != "dagou-bang"
	 || busy < 1 ){
		if (target && target->query_temp("dgb/chan")) {
			target->delete_temp("dgb/chan", 1);
			target->start_busy(-1);
			target->interrupt_me();
		}
           if(!userp(me) )
			me->start_perform(1, "缠字诀");
		return;
	}
	call_out("check_fight", 1, me,target,weapon,busy -1);
}

int help(object me)
{
	write(HBGRN"\n打狗棒「缠字诀」："NOR"\n");
	write(@HELP
	昔年洪七公以打狗棒之轻灵奇巧，跻身当世五大高手。
	这打狗棒「缠字诀」，正是洪七公当年惯使的一招成名绝技。
	那施展出浑身解数，也脱不开的如梦幻般的棒影，令对手心惊肉跳。

	要求：	最大内力 1000 以上；
		当前内力 500 以上；
		打狗棒等级 100 以上；
		混天气功等级 100 以上；
		打狗棒诀等级 100 以上：
		激发招架为打狗棒。
HELP
	);
	return 1;
}
