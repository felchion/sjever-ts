//chaodu.c  般若掌超度连环诀

#include <ansi.h>

inherit F_DBASE;
inherit F_SSERVER;

int query_ap() {return 3;}

string perform_name() { return HIY"超度诀"NOR; }
	
int perform(object me, object target)
{
	int i,lvl;

	if( !target ) target = offensive_target(me);

	if( !target
	||  !me->is_fighting(target) )
	return notify_fail("「超度诀」只能对战斗中的对手使用。\n");

	if( objectp(me->query_temp("weapon")))
	return notify_fail("你手中拿着武器，如何能够施展「超度诀」？\n");

	if( me->query_skill_mapped("force") != "yijin-jing" )
	return notify_fail("你所用的并非易筋经神功，无法配合掌法施展「超度诀」！\n");

	if( me->query_skill("yijin-jing") < 120 )
	return notify_fail("你的易筋经气功火候未到，无法配合施展「超度诀」！\n");
	
	if( me->query_skill("banruo-zhang") < 120 )
	return notify_fail("你掌法修为不足，还不会使用「超度诀」！\n");


	if (me->query_skill_mapped("strike") != "banruo-zhang" &&
	me->query_skill_prepared("strike") != "banruo-zhang")	
	return notify_fail("你现在无法使用「超度诀」进行攻击。\n");
	
	if( me->query("max_neili") < 2000 )
	return notify_fail("你现在内力太弱，使不出「超度诀」。\n");

	if( me->query("neili") < 1000 )
	return notify_fail("你的内力不够施展连环诀！\n");


	lvl = me->query_skill("banruo-zhang",1);

	message_vision(HIR "\n$N大喝一声，运掌如电,掌风霍霍，招招连环，正是佛门般若超度诀！\n" NOR, me, target);
  message_vision(HIC"$N运起超度决一招之间闪电般地劈出凌历的两掌。\n"NOR, me);

	me->add("neili", -300-lvl/5);
	me->add_temp("apply/pfm_attack", 180+lvl/10);
	me->add_temp("apply/damage", 300+lvl/2);
	me->add_temp("apply/pfm_damage", 180+lvl/5);

	for(i = 0; i < 2; i++)
	  if ( objectp(target) && me->is_fighting(target))
	    COMBAT_D->do_attack(me, target,0, random(3)?1:3);

	me->add_temp("apply/pfm_attack", -180-lvl/10);
	me->add_temp("apply/damage", -300-lvl/2);
	me->add_temp("apply/pfm_damage", -180-lvl/5);
	me->start_busy(random(2));

	return 1;
}


int help(object me)
{
        write(HIY"\n「超度诀」："NOR"\n");
        write(@HELP        

        要求：  最大内力 2000 以上；
                当前内力 1000 以上；
                般若掌等级 120 以上；
                易筋经等级 120 以上；
                激发散般若掌为掌法；
                
HELP
        );
        return 1;
}