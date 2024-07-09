// By Daidai 2002/03/16

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 3;}

string perform_name(){ return HIR"穿云追日"NOR; }

int perform(object me, object target){
	int lvl;	
	
	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target))
		return notify_fail("「穿云破日」只能对战斗中的对手使用。\n");

	if (me->query_skill_prepared("leg") != "chuanyun-tui"
	|| me->query_skill_mapped("leg") != "chuanyun-tui")
		return notify_fail("你现在无法使用「穿云破日」！\n");
	
	if((int)me->query_skill("xuantian-wuji",1) < 120)
		return notify_fail("你玄天无极功的功力不够不能使用「穿云破日」!\n");

	if( (lvl = (int)me->query_skill("chuanyun-tui",1)) < 120)
		return notify_fail("你的穿云腿修为不够,目前还不能使用「穿云破日」绝技!\n");

	if((int)me->query("max_neili") < 1300)
		return notify_fail("你内力修为不够, 不能使用「穿云破日」! \n");
	
	if( me->query_temp("weapon"))
		return notify_fail("你拿着武器怎么能使用「穿云破日」！\n");

	if((int)me->query("neili") < 1000)
		return notify_fail("你内力现在不够, 不能使用「穿云破日」! \n");
	

	message_vision( HIC"\n$N霍地借力一弹，冲天而起，再以穿云破日之势，如九天星坠般直射而下。\n"NOR, me, target);

	me->add("neili", -500-lvl/2 );	

	me->add_temp("apply/pfm_force", 120 + lvl/5);
	me->add_temp("apply/force", 200 + lvl/2);
	me->add_temp("apply/pfm_attack", 100 + lvl/10);
	me->add_temp("apply/action/color", HIC);

	COMBAT_D->do_attack(me, target);
	if (present(target,environment(me)) && me->is_fighting(target)){
		COMBAT_D->do_attack(me, target);
  }
  if (present(target,environment(me)) && me->is_fighting(target) ){
		COMBAT_D->do_attack(me, target, 0,3);
  }
  
	me->add_temp("apply/pfm_force", -120 - lvl/5);
	me->add_temp("apply/force", -200 - lvl/2);
	me->add_temp("apply/pfm_attack", -100 -lvl/10);
	me->delete_temp("apply/action/color");
	me->start_busy(1);

	return 1;
}

int help(object me)
{
        write(HIC"\n穿云腿之「穿云破日」："NOR"\n");
        write(@HELP

        要求：  最大内力 1000 以上；
                当前内力 1000 以上；
                穿云腿 120 以上；
                玄天无极功等级 120 以上；
                
HELP
        );
        return 1;
}
