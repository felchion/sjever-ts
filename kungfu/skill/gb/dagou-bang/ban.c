// ban.c 打狗棒法--绊字诀
// Creat by Looklove@SJ 3/16/2001
// modified by Looklove 2001.5.10
// Modified by Lane@SJ 2005.7.29

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;


int query_ap() {return 4;}

string perform_name(){ return HBBLU"绊字诀"NOR; }
int perform(object me, object target)
{
	object weapon;
	int lvl;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target) )
		return notify_fail("「绊字诀」只能对战斗中的对手使用。\n");

	weapon = me->query_temp("weapon");

	if( !weapon
	 || weapon->query("skill_type") != "stick"
	 || me->query_skill_mapped("stick") != "dagou-bang" 
	 || me->query_skill_mapped("parry") != "dagou-bang" )
		return notify_fail("你现在无法使用「绊字诀」。\n");


	if( (lvl = me->query_skill("dagou-bang", 1) )< 140 )
		return notify_fail("你的打狗棒法还不够娴熟，不会使用「绊字诀」。\n");

	if( me->query_skill("bangjue",1) < 140 )
		return notify_fail("你的打狗棒诀不够娴熟，无法使出「绊字诀」。\n");

	if( (int)me->query_skill("huntian-qigong",1) < 140 )
		return notify_fail("你的混天气功根基不足，无法使出「绊字诀」。\n");

	if( (int)me->query("neili") < 600 )
		return notify_fail("你的内力不够，无法使出「绊字诀」。\n");

	if( (int)me->query("jingli") < 400 )
		return notify_fail("你的精力不足以让你使出「绊字诀」。\n");

	if( (int)me->query("max_neili") < 2000 )
		return notify_fail("你内力修为现在不够，不能用「绊字诀」！\n");

	if( target->query_temp("dgb/ban") )
		return notify_fail("对方已经被拌中了，放手攻击吧。\n");

	me->set_temp("apply/action/pfm",HBBLU"$N"+weapon->name()+HBBLU"挥出，以「绊字诀」着地扫去，一绊不中，二绊续至，连环钩盘，绵绵而不绝");
	me->set_temp("apply/action/hit", HBBLU"$n纵身稍慢，被"+weapon->name()+HBBLU"在左胫上一拨，右踝上一钩，扑地倒了。\n"NOR);
  target->set_temp("apply/action/dodge",HIG"$n下盘功夫十分扎实，闪展腾挪间躲开"+weapon->name()+HIG"的绊绕，顺势一脚踢中$N的屁股。\n"NOR);
  target->set_temp("apply/action/parry",HIG"$n下盘功夫十分扎实，闪展腾挪间躲开"+weapon->name()+HIG"的绊绕，顺势一脚踢中$N的屁股。\n"NOR);

	me->add_temp("apply/pfm_hurt",350+lvl/2);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	if(COMBAT_D->do_attack(me, target,0,random(2)?0:3,2)) {
		target->set_temp("dgb/ban", 1);
		target->add_temp("apply/pfm_dodge",-120);
		target->apply_condition("dgb_ban_wound", 10);
		call_out("check_perform", 1, me,target,60);
	}
	me->add_temp("apply/pfm_hurt",-350-lvl/2);
	me->add_temp("apply/pfm_attack",-180-lvl/10);	  
	me->delete_temp("apply/action");
	target->delete_temp("apply/action");
	me->start_busy(random(2));

	return 1;
}

void check_perform(object me,object target, int count)
{
	if ( !target || !target->query_temp("dgb/ban")) return;
	if ( !me || target->is_fighting(me) || count<1) {
		target->add_temp("apply/pfm_dodge", 120);
		target->delete_temp("dgb/ban");
		return;
	}
	call_out("check_perform", 1,me, target, count--);
}

int help(object me)
{
        write(HBBLU"\n打狗棒「绊字诀」："NOR"\n");
        write(@HELP
        打狗棒以轻、巧、灵诸称，舞将起来是四处只见棒影不见人影。
        这「绊字诀」，却是专攻敌人下盘的招数。对于下盘不稳的对手
        极是有效，一旦绊中，对手身形变慢，必处处受制。

        要求：  最大内力 2000 以上；
                当前内力 600 以上；
                打狗棒等级 140 以上；
                混天气功等级 140 以上；
                打狗棒诀等级 140 以上：
                激发招架为打狗棒。
HELP
        );
        return 1;
}
