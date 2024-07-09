// lifeheal.c

#include <ansi.h>

int query_ap() {return 3;}

string exert_name(){ return HIG"无极心法"NOR; }

int exert(object me, object target)
{
	int skill;

	if (!me->is_fighting())
	  return notify_fail("你只能在战斗中使用无极心法。\n");

	skill = me->query_skill("force",1);

	if ( me->query_skill_mapped("force") != "xuantian-wuji")
		return notify_fail("你现在的情况，无法使用无极心法。\n");
	
	if( (int)me->query_skill("xuantian-wuji",1) < 100 )
  	return notify_fail("你目前尚不能使用无极心法。\n");

	if( (int)me->query_temp("kl/riyue") )
  	return notify_fail("你已经在运用无极心法了。\n");

	if (me->query("neili") < 1000)
		return notify_fail("你的内力不够，无法使用无极心法。\n");
		
	me->add("neili", -1 * skill);
	me->add_temp("apply/pfm_parry", 240);

	message_vision(HIW "$N微一凝神，运起无极心法，只见$N全身被一团真气围绕。\n" NOR, me);

	me->set_temp("kl/riyue", 1);
	call_out("remove_effect", 1, me, skill);
	return 1;
}

void remove_effect(object me, int skill)
{
	if (objectp(me)) {
	  if (me->is_fighting()){
	      call_out("remove_effect", 1, me, skill);
	      return;
	  }
	  me->delete_temp("kl/riyue");
	  me->add_temp("apply/pfm_parry", -240);
  	tell_object(me, HIW"你的玄天无极功运行完毕，将内力收回丹田。\n"NOR);
	}
	return;
}

int help(object me)
{
        write(HIG"\n玄天无极功之「无极心法」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                玄天无极功等级 100 以上；
                激发内功为玄天无极功。

HELP
        );
        return 1;
}