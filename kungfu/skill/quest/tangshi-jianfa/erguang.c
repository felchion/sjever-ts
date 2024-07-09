//erguang.c 耳光式
// By Spiderii

#include <ansi.h>

inherit F_SSERVER;

int perform(object me, object target)
{
	object weapon;
	int j, lvl = me->query_skill("tangshi-jianfa", 1);

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target)
	 || !living(target) )
		return notify_fail("「耳光式」只能对战斗中的对手使用。\n");

	
   if( (int)me->query_skill("tangshi-jianfa", 1) < 80 )
		return notify_fail("你的躺尸剑法功力太浅，别做梦了。\n");

   if( (int)me->query_skill("sword", 1) < 80 )
		return notify_fail("你的基本剑法功力太浅，别做梦了。\n");


   if( (int)me->query("max_neili") < 500 )
		return notify_fail("你的内力太浅，别做梦了。\n");

	if( (int)me->query("neili") < 100 )
		return notify_fail("你的真气不够，别做梦了。\n");

	
	if( !objectp(weapon = me->query_temp("weapon"))
	 || weapon->query("skill_type") != "sword"
	 || me->query_skill_mapped("sword") != "tangshi-jianfa" )
		return notify_fail("你使得了「耳光式」么?\n");

	j = lvl/3;

	if ( lvl > 275 )
		j = to_int(lvl/3.0 * lvl/275.0);

	me->set_temp("tangshi/erguang",1);

	me->add_temp("apply/sword", j);
       me->add_temp("apply/attack", j);
       me->add_temp("apply/damage", j/2);
	
       COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);

	me->add_temp("apply/sword", -j);
       me->add_temp("apply/attack", -j);
	me->add_temp("apply/damage", -j/2);

	me->delete_temp("tangshi/erguang");

	me->start_busy(random(2));
	me->add("neili", -300);
	me->add("jingli", -100);
	me->start_perform(2, "「耳光式」");
	return 1;
}

string perform_name(){ return HIW"耳光式"NOR; }

int help(object me)
{
        write(HIW"\n躺尸剑法之「耳光式」："NOR"\n\n");
        write(@HELP
       躺尸剑法极其精妙的一招。
　　　　
　　　　要求：　当前内力 100 以上；
　　　　　　最大内力 500 以上；
　　　　　　　　躺尸剑法等级 80 以上；
　　　　　　　　基本剑法等级 80 以上；
　　　　　　　　激发招架为躺尸剑法。

HELP
        );
        return 1;
}
