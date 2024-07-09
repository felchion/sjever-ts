// qiankun.c 
// tiantian@sj 2000.10.14 
// Modified by snowman@SJ 12/12/2000

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

int query_ap() {return 4;}

string perform_name() { return HIW"袖里乾坤"NOR; }

int perform(object me, object target)
{
	string msg;
	int improve;

	if( !target ) target = offensive_target(me);

	if( !objectp(target))
	return notify_fail("你要对谁使用「袖里乾坤」？\n");

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target))
	return notify_fail("「袖里乾坤」只能对战斗中的对手使用。\n");

	if( me->query("family/family_name")!="少林派" )
	return notify_fail("你不是少林弟子，如何能使用少林绝技？\n");

	if( objectp(me->query_temp("weapon")) )
	return notify_fail("你必须空手使用「袖里乾坤」！\n");

	if( (int)me->query_skill("wuxiang-zhi",1) < 200 )
	return notify_fail("你的无相劫指不够娴熟，不会使用「袖里乾坤」！\n");

	if( (int)me->query_skill("yijin-jing",1) < 200 )
	return notify_fail("你的易筋经内功等级不够，不能使用「袖里乾坤」！\n");

	if( (int)me->query("max_neili") < 3000 )
	return notify_fail("你的内力太弱，不能使用「袖里乾坤」！\n");

	if( (int)me->query("neili") < 1000 )
	return notify_fail("你的内力太少了，无法使用出「袖里乾坤」！\n");

	if (me->query_skill_mapped("finger") != "wuxiang-zhi" &&
	me->query_skill_prepared("strike") != "wuxiang-zhi")	
	return notify_fail("你现在无法使用「袖里乾坤」进行攻击。\n");

	if( target == me )
	return notify_fail("自己点自己啊！\n");

	if (target->is_corpse() || !target->is_character())
	return notify_fail("那不是活物耶！\n");
	
	if(!living(target))
	return notify_fail("对方都这样了，还用的着这么费劲么？\n");

	if(  target->is_busy() )
	return notify_fail("对方正自顾不暇，放胆攻击吧！\n");

	me->add("neili", -800);
	tell_object(me, HIY"你面带微笑，将双手拢在衣袖之中，指力从衣袖中暗暗发出，一股若有若无的微风向着"
	+target->name()+"袭去。\n" NOR);
	me->start_call_out( (: call_other, __FILE__, "remove_effect", me, improve :), 6 );
	if(COMBAT_D->do_attack(me, target, 0, 0, 1) 
	|| COMBAT_D->do_attack(me, target, 0, 0, 1)){
		tell_object(me,"结果"+target->query("name")+"被你指风点中，登时动弹不得。\n");
		tell_object(target,"你只觉得一股若有若无的微风从身上拂过，不经意间发现自己已经无法动弹。\n");

		target->start_busy(4 + random(me->query_skill("wuxiang-zhi",1))/40);
	}
	else {
			msg = HIY"$n只觉得一股若有若无的微风从身上拂过，却又不知道发生了什么事情。\n"NOR;
			me->start_busy(1+random(2));	
			message_vision(msg, me, target);
	}
	return 1;
}

int help(object me)
{
        write(HIW"\n「袖里乾坤」："NOR"\n");
        write(@HELP
        

        要求：  最大内力 3000 以上；
                当前内力 1000 以上；
                无相劫指等级 200 以上；
                易筋经等级 180 以上；
                激发散无相劫指为指法；
                
HELP
        );
        return 1;
}