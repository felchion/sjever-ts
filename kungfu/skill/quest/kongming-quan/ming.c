// ming.c  
// by tiantian@sj 2000.6.19

#include <ansi.h>

inherit F_SSERVER;

int perform(object me,object target)
{
  int lv;  
	
	if (!target) target = offensive_target(me);
	
	if (!target || !me->is_fighting(target))
		return notify_fail("「明字诀」只能在战斗中使用。\n");                
  
  if( me->query_temp("weapon") )
    return notify_fail("你必须空手使用「明字诀」！\n");
    
  if (me->query_skill_prepared("cuff") != "kongming-quan"
  || me->query_skill_mapped("cuff") != "kongming-quan"
  || me->query_skill_mapped("cuff") != "kongming-quan" )
    return notify_fail("你现在无法使用「明字诀」。\n");	
	
	if (lv=me->query_skill("kongming-quan", 1) < 150)
		return notify_fail("你的空明拳尚不纯熟，还不能使用「明字诀」。\n");        
	if (me->query("neili") < 1000)
		return notify_fail("你现在内力不足，不能使用「明字诀」！\n");    
  if (!me->query("quest/jiebai/pass"))
  	return notify_fail("你没有和周伯通结义，不能使用「明字诀」。\n");
	
	if (me->query_temp("kongming/ming"))
		return notify_fail("你正在使用「明字诀」！\n");      	
	
	message_vision(HIW"\n$N身形一变，$n猛觉得对方拳力若有若无，自己招式使实了固然不对，使虚了也是极其危险，不禁暗暗吃惊。\n"NOR, me,target);
	
	me->add("neili", -200 - lv/2); 
	me->start_perform(4,"明字诀");
	me->set_temp("kongming/ming", 1);
	me->add_temp("apply/pfm_parry",70+lv/10);
  me->add_temp("apply/pfm_dodge",70+lv/10);
	call_out("check_fight", 1, me, lv, 10+me->query_skill("kongming-quan",1)/10);
	return 1;
}

void remove_effect(object me,int lv);
void check_fight(object me, int lv, int count){

	if (!me) return;
	if( me->query_temp("weapon")
	|| me->query_skill_prepared("cuff") != "kongming-quan"
  || me->query_skill_mapped("cuff") != "kongming-quan"
  || me->query_skill_mapped("cuff") != "kongming-quan"
	|| !me->is_fighting()
	|| count < 1 ){
		remove_effect(me, lv);
		return;
	}
	call_out("check_fight", 1, me, lv, count-1);
}

void remove_effect(object me,int lv)
{
	if (objectp(me)) 
	{
		me->delete_temp("kongming/ming");
		me->add_temp("apply/pfm_parry",-70-lv/10);
  	me->add_temp("apply/pfm_dodge",-70-lv/10);
		tell_object(me, HIW"\n你缓缓呼气，将内力纳入丹田。\n"NOR);
	}
}

string perform_name(){ return HIW"明字诀"NOR; }

int help(object me)
{
	write(HIW"\n空明拳之「明字诀」："NOR"\n\n");
	write(@HELP
	要求：	
		当前内力 1000 以上；
		空明拳等级 150 以上；
		激发拳法为空明拳；
		激发招架为空明拳；
		备拳法为空明拳且无兵器。

HELP
	);
	return 1;
}