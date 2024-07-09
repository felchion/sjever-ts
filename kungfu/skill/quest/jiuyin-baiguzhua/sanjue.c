// jiuyin 九阴三绝爪
// By Spiderii 适当提高
#include <ansi.h>
#include <combat.h> 
inherit F_SSERVER;

int query_ap() {return 3;}

int perform(object me, object target)
{
	int lvl;
	lvl = (int)me->query_skill("jiuyin-baiguzhua", 1);

	if( !target ) target = offensive_target(me);

  if( !me->query("quest/jiuyin2/pass") && !me->query("quest/jiuyin1/pass") )
		return notify_fail("你无法使用「九阴三绝爪」。\n");
		
  if( !objectp(target) || !me->is_fighting(target) || !necessary(target) )
		return notify_fail("「九阴三绝爪」只能在战斗中使用。\n");

	if( me->query_temp("weapon"))
		return notify_fail("你拿着武器怎么能使用「九阴三绝爪」！\n");   

	if( lvl < 150 || me->query_skill("claw", 1) < 150)
		return notify_fail("你的九阴白骨爪还不够娴熟，使不出「九阴三绝爪」绝技。\n");

	if ( me->query_skill_mapped("claw") != "jiuyin-baiguzhua"
	|| me->query_skill_prepared("claw") != "jiuyin-baiguzhua")
		return notify_fail("你现在无法使用「九阴三绝爪」！\n");

	if( (int)me->query("max_neili") < 1500  )
		return notify_fail("你现在内力太弱，使不出「九阴三绝爪」。\n");      

	if( (int)me->query("neili") < 1100 )
		return notify_fail("你现在真气太弱，使不出「九阴三绝爪」。\n");

	if( me->query("shen") > -1000000 )
			return notify_fail("你不够狠毒，无法使出「九阴三绝爪」的绝技。\n");

	message_vision(RED"\n$N突然怪叫一声，全身骨骼如爆豆般噼啪作响，手臂突然暴涨三尺！\n"NOR,me);

  		
	me->add_temp("apply/pfm_damage", 150+lvl/5);
	me->add_temp("apply/damage", 200+lvl);
	me->add_temp("apply/pfm_attack", 80+lvl/10);
	me->set_temp("apply/action/color", RED);
	
	me->add("neili", -400);
	me->add("jingli", -150);	

	COMBAT_D->do_attack(me, target, 0, random(3)?3:1);
	
	if (present(target,environment(me)) && me->is_fighting(target))
		COMBAT_D->do_attack(me, target, 0, random(2)?1:3);
	
	if (present(target,environment(me))
	&& me->is_fighting(target)
	&& me->query_skill("jiuyin-baiguzhua", 1) > 200)
		COMBAT_D->do_attack(me, target, 0, 3);
	
	me->add_temp("apply/pfm_damage", -150-lvl/5);
	me->add_temp("apply/damage", -200-lvl);
	me->add_temp("apply/pfm_attack", -80-lvl/10);
	me->delete_temp("apply/action/color");
	

	me->start_busy(random(2));
	return 1;
}

string perform_name(){ return RED"九阴三绝爪"NOR; }

int help(object me)
{
        write(RED"\n九阴白骨爪之「九阴三绝爪」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1100 以上；
                最大内力 1500 以上；
                当前精力 1000 以上；
                九阴白骨爪等级 150 以上；
                基本抓法等级 150 以上；
                负神 1000000 以上；
                激发抓法为九阴白骨爪；                
                备抓法为九阴白骨爪；
                且手无兵器。

HELP
        );
        return 1;
}
