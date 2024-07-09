// shield.c
// changed by emnil@sj  2/2/2k
// changed by hunthu@sj 2/20/2001
#include <ansi.h>
inherit F_CLEAN_UP;

int query_ap() {return 2;}

int exert(object me, object target)
{
	int skill;

	if ( !me->is_fighting())
		return notify_fail("你只有在战斗中，才能运气盾。\n");
		
	if ((int)me->query_skill("longxiang-boruo", 1) < 100)
		return notify_fail("你的龙象般若功修为还不够。\n");

	if((int)me->query("max_neili") < 1500)
		return notify_fail("你的内力修为太差，多多打坐吧。\n");

	if( (int)me->query("neili") < 1000 ) 
		return notify_fail("你的真气不够。\n");

	if (me->query_temp("xs/shield"))
		return notify_fail("你已经在运真气护体了。\n"); 

	skill = me->query_skill("longxiang-boruo",1) / 4;

	message_vision(CYN "$N深深吸入一口气，体内真气流转，身周仿佛凝结了一面气盾！\n" NOR, me);

	me->add_temp("apply/pfm_dodge", skill);
	me->add_temp("apply/eq_armor", skill);
	me->set_temp("xs/shield", skill);
	me->add("neili", -300);

	call_out("remove_effect", 1, me, skill);
	if( me->is_fighting() ) me->start_busy(random(2));
	return 1;
}

void remove_effect(object me,int count)
{
	int amount;

	if ( !me || !me->query_temp("xs/shield") ) return;
	
	if (count < 1 || !me->is_fighting()) {
		amount = me->query_temp("xs/shield");
		me->add_temp("apply/pfm_dodge", - amount);
		me->add_temp("apply/eq_armor", - amount);
		me->delete_temp("xs/shield");
		message_vision(RED "$N脸上红光一闪，骨节啪啪轻响几声，散去了护体真气。\n" NOR, me);
	}
	call_out("remove_effect", 1, me, count - 1);
}

string exert_name(){ return CYN"气盾"NOR; }

int help(object me)
{
        write(CYN"\n龙象般若功之「气盾」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 1500 以上；
                龙象般若功等级 100 以上。

HELP
        );
        return 1;
}
