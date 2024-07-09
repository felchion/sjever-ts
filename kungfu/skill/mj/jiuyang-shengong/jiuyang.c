// jiuyang.c 九阳神功加力

#include <ansi.h>

inherit F_CLEAN_UP;

void remove_effect(object me, int amount);

int query_ap() {return 12;}

string exert_name(){ return HIR"九阳神功"NOR; }

int exert(object me)
{
	if(!me->is_fighting()) 
	     return notify_fail("你只能在战斗中用九阳神功来提升自己的战斗力。\n");

	if( (int)me->query("neili") < 2000  ) 
		return notify_fail("你的内力不够。\n");
		
	if( (int)me->query_temp("jiuyang/powerup") ) 
		return notify_fail("你已经在运功中了。\n");

	me->add("neili", -1500);
	message_vision(HIR "$N微一凝神，运起九阳神功，只见$N的脸色变得红润多了。\n" NOR, me);
	me->set_temp("jiuyang/powerup",1);
	me->add_temp("apply/pfm_parry", 120);
	me->add_temp("apply/pfm_dodge", 120);
	me->add_temp("apply/pfm_attack", 300);
	me->add_temp("apply/pfm_damage", 300);
	me->set_temp("jiuyang/damage", 1000);
	call_out("remove_effect", 1, me, 25);
	me->start_busy(random(2));
	return 1;
}

void remove_effect(object me, int amount)
{
	if (! me) return;
	if ( amount < 1){
		me->add_temp("apply/pfm_parry", -120);
		me->add_temp("apply/pfm_dodge", -120);
		me->add_temp("apply/pfm_attack", -300);
		me->add_temp("apply/pfm_damage", -300);
		me->set_temp("jiuyang/damage", -1000);
		me->delete_temp("jiuyang/powerup");
		
		tell_object(me, HIW"\n你的"HIR"九阳神功"HIW"运行完毕，将内力收回丹田。\n"NOR);
		return;
	}
	call_out("remove_effect", 1, me, amount -1);
}
