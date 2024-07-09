// powerup.c 混天气功加力
// Modify by Looklove 2001.5.3

#include <ansi.h>

inherit F_CLEAN_UP;

int query_ap() {return 1;}

string exert_name(){ return HIW"爆骨功"NOR; }

int exert(object me, object target)
{
	
	if( !me->is_fighting() )
		return notify_fail("混天气功只能在战斗中使用。\n");
		
	if( (int)me->query_skill("force") < 300 )
		return notify_fail("你的内功等级不够，不能使用混天气功。\n");
		
	if( (int)me->query("neili") < 1000  )
		return notify_fail("你的内力不够。\n");
	if (me->query_skill_mapped("force") != "huntian-qigong")
		return notify_fail("你的内功不对。\n");
	if(me->query_temp("htqg/powerup"))
		return notify_fail("你已经在运功中了。\n");

	me->receive_damage("neili", -(700+random(300)));
	message_vision(HIR "$N微一凝神，运起混天气功，全身骨节发出一阵爆豆般的声响。\n" NOR, me);

	me->add_temp("apply/pfm_damage", 100);
  me->set_temp("htqg/powerup",1);
  
	call_out("remove_effect",1, me,45+random(30));
	return 1;
}

void remove_effect(object me, int count)
{
	int amount;
	if (objectp(me) && me->query_temp("htqg/powerup")) {
		if (count > 0 && me->is_fighting()) {
			call_out("remove_effect", 1, me, count--);
			return;
		}
		amount = me->query_temp("htqg/powerup");
		me->add_temp("apply/pfm_damage", -100);
		me->delete_temp("htqg/powerup");
		tell_object(me, HIR"你的混天气功运行完毕，将内力收回丹田。\n"NOR);
	}
}

int help(object me)
{
	write(@HELP
	要求：	
		当前内力 1000 以上；
		内功有效等级 300 以上；
HELP
	);
	return 1;
}