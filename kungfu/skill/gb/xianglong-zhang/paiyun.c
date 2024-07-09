// 排云双掌
// by snowman@SJ
// Modify by Lklv for npc can't add gift
// Modify by Lane@SJ 2005.08.03 
// Update by lsxk@hsbbs 2007/7/3

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 5;}

string perform_name(){ return WHT"排云双掌"NOR; }

int perform(object me, object target)
{
	int lvl;

	lvl = me->query_skill("xianglong-zhang", 1);

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !me->is_fighting(target)
	 || !living(target)
	 || environment(target) != environment(me) )
		return notify_fail("「排云双掌」只能在战斗中对对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
		return notify_fail("你必须空手才能使用「排云双掌」！\n");

	if( lvl < 150 )
		return notify_fail("你的降龙十八掌还不够娴熟，使不出「排云双掌」绝技。\n");

	if( (int)me->query_skill("huntian-qigong", 1) < 150 )
		return notify_fail("你的混天气功等级还不够，使不出「排云双掌」绝技。\n");

	if( me->query_skill_mapped("force") != "huntian-qigong" )
		return notify_fail("你所使的内功不对，使不出「排云双掌」绝技。\n");

	if( (int)me->query_str(1) < 36 )
		return notify_fail("你的膂力还不够强劲，使不出「排云双掌」来。\n");

	if( me->query_skill_prepared("strike") != "xianglong-zhang"
	 || me->query_skill_mapped("strike") != "xianglong-zhang"
	 || me->query_skill_mapped("parry") != "xianglong-zhang")
		return notify_fail("你现在无法使用「排云双掌」！\n");

	if( (int)me->query("max_neili") < 2000 )
		return notify_fail("你现在内力太弱，使不出「排云双掌」。\n");

	if( (int)me->query("neili") < 1500 )
		return notify_fail("你现在真气太弱，使不出「排云双掌」。\n");

	if( (int)me->query("jingli") < 800 )
		return notify_fail("你的精力不足以让你使出「排云双掌」。\n");


	message_vision(HIW"\n$N一声断喝，双掌齐出，挟起满天飞沙！使的正是萧峰的得意功夫「排云双掌」！\n"NOR,me);


	me->add_temp("apply/action/color",HIY);
	me->receive_damage("neili", 500+lvl);
	
  me->add_temp("apply/damage",500+lvl);
	me->add_temp("apply/pfm_damage",250+lvl/4);	
	me->add_temp("apply/pfm_attack",240+lvl/10);	
	
	COMBAT_D->do_attack(me, target,0,random(2)?0:3);

	if( me->is_fighting(target) )
     COMBAT_D->do_attack(me, target, me->query_temp("weapon"),random(2)?0:3);
  
  me->add_temp("apply/damage",-500-lvl);
	me->add_temp("apply/pfm_damage",-250-lvl/4);	
	me->add_temp("apply/pfm_attack",-240-lvl/10);		

	me->delete_temp("apply/action/color"); 
	me->start_busy(random(2));

	return 1;
}



int help(object me)
{
	write(WHT"\n降龙掌「排云双掌」："NOR"\n");
	write(@HELP
	要求：	最大内力 2000 以上；
		当前内力 1500 以上；
		当前精力 800 以上；
		降龙掌等级 150 以上；
		混天气功等级 150 以上；
		膂力 36 以上：
		激发招架为降龙掌。
HELP
	);
	return 1;
}
