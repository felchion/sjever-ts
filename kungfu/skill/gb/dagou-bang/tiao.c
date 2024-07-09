// tiao.c 打狗棒法--挑字诀
// Creat by looklove@SJ 3/17/2001

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

int query_ap() {return 5;}

string perform_name(){ return HBMAG"挑字诀"NOR; }
int perform(object me, object target)
{
	string msg;
	object tweapon, weapon;

	me->clean_up_enemy();
	target = me->select_opponent();

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target) )
		return notify_fail("「挑字诀」只能对战斗中的对手使用。\n");

	weapon = me->query_temp("weapon");
	tweapon = target->query_temp("weapon");

	if( !weapon
	 || weapon->query("skill_type") != "stick"
	 || me->query_skill_mapped("stick") != "dagou-bang" )
		return notify_fail("你现在无法使用挑字诀。\n");

	if( !tweapon )
		return notify_fail("对方没有使用任何兵器，你无法使用「挑字诀」。\n");

	if( me->query_skill("dagou-bang",1) < 160 )
		return notify_fail("你的打狗棒法还不够娴熟，不会使用「挑字诀」。\n");

	if( me->query_skill("bangjue",1) < 160 )
		return notify_fail("你的打狗棒诀不够娴熟，无法使出「挑字诀」。\n");

	if( (int)me->query_skill("huntian-qigong", 1) < 160 )
		return notify_fail("你的混天气功根基不足，无法使出「挑字诀」。\n");

	if( (int) me->query("neili") < 800 )
		return notify_fail("你的内力不够，无法使出「挑字诀」。\n");

	msg = HBMAG"$N捏个棒诀将"+weapon->name()+HBMAG"一转，轻喝一声“起！”，使出「挑字诀」，搭住"+tweapon->name()+HBMAG"向上挑出。\n"NOR;

	if( COMBAT_D->do_attack(me, target, weapon, 0, 1) 
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)) {
		int move_flag = 0;
		if(tweapon->is_owner(target)) move_flag=1;
		msg += HBMAG"$n只感"+tweapon->name()+HBMAG"似欲脱手飞出，一个把握不住，手中兵器被挑飞了出去。\n"NOR;
		target->add_busy(10);
		me->add("neili",-300);

		if (move_flag==0 && random(2) && tweapon->query("imbued") < 3 && tweapon->move(me)){
			msg += HIY"$N提气纵身，跃起丈余，将半空中的"+tweapon->name()+HIY"抢在手中。\n"NOR;
		}
		else {
			msg += HBMAG"$n眼望下落的"+tweapon->name()+HBMAG"伸手欲接，被$N"+weapon->name()+HBMAG"又一番疾攻。只好任"+tweapon->name()+HBMAG"落在一边。\n"NOR;
			tweapon->move(environment(me));
		}
	}
	else {
		me->add("neili",-200);
		msg += HIG"$n运力于腕，斜下里一荡，将"+weapon->name()+HIG"从自己的"+tweapon->name()+HIG"上荡了开去。\n"NOR;
	  me->start_busy(random(2));
	}
	
	message_vision(msg, me, target);
	return 1;
}

int help(object me)
{
	write(HBMAG"\n打狗棒「挑字诀」："NOR"\n");
	write(@HELP
	打狗棒法集轻灵奇巧于一身，临敌之际纵不得伤敌，亦得自保。
	若对手手持兵器，则使用此「挑字诀」，可将对手兵器打飞或
	者挑入自己手中。

	要求：	当前内力 800 以上；
		打狗棒等级 160 以上；
		混天气功等级 160 以上；
		打狗棒诀等级 160 以上：
		激发招架为打狗棒；
		对手若手无兵器，则无效果。
HELP
	);
	return 1;
}