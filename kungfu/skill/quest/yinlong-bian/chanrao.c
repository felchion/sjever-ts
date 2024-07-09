#include <ansi.h>

inherit F_SSERVER;

int perform(object me, object target)
{
	string msg;
	object weapon = me->query_temp("weapon");
	int i;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
		return notify_fail("缠绕诀只能对战斗中的对手使用。\n");

	if( !me->query("quest/jiuyin2/pass") && !me->query("quest/jiuyin1/pass") )
		return notify_fail("你无法使用「缠绕诀」。\n");
		
	if(me->query_skill("yinlong-bian",1) < 130)
		return notify_fail("你的九阴银龙鞭还不够娴熟，不会使用缠绕诀。\n");

	if( !weapon 
	|| weapon->query("skill_type") != "whip"
	|| me->query_skill_mapped("whip") != "yinlong-bian" 
	|| me->query_skill_mapped("parry") != "yinlong-bian" ) 
		return notify_fail("你现在无法使用缠绕诀。\n");

	if( me->query("neili") < 1500 )
		return notify_fail("你的内力不够。\n");

	if( target->is_busy() )
		return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧。\n");

	msg = HIG "\n$N使出缠绕诀，鞭子一挥向$n的双腿卷去！\n";
  
  i = 2+me->query_skill("yinlong-bian",1)/100;

	if( COMBAT_D->do_attack(me, target, 0, 0, 1)
	|| COMBAT_D->do_attack(me, target, 0, 0, 1) ) {
		msg += HIY "结果$p被$P一卷，两条腿被结结实实的捆起来，在地上无法爬起。\n" NOR;
		target->start_busy(i);
	}
	else {
		msg += HIW "可是$p看破了$P的缠绕诀，身子一跃便躲开了。\n" NOR;
		me->start_busy(2+random(2));
	}
	message_vision(msg, me, target);

	
	me->add("jingli", -100);
	me->add("neili", -1000);
  return 1;
}

string perform_name(){ return HIG"缠绕诀"NOR; }

int help(object me)
{
        write(HIG"\n九阴银龙鞭之「缠绕诀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1500 以上；
                最大内力 1500 以上；
                当前精力 1500 以上；
                九阴银龙鞭等级 130 以上；
                激发鞭法为九阴银龙鞭；
                激发招架为九阴银龙鞭；
                且手持兵器。

HELP
        );
        return 1;
}
