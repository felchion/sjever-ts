// kong.c  
// by tiantian@sj 2000.6.19

#include <ansi.h>

inherit F_SSERVER;

int perform(object me,object target)
{
	int lv;
	
	if (!target) target = offensive_target(me);
	
	if (!target || !me->is_fighting(target))
		return notify_fail("「空字诀」只能在战斗中使用。\n");                

	if( me->query_temp("weapon") )
    return notify_fail("你必须空手使用「空字诀」！\n");
    
	if (me->query_skill_prepared("cuff") != "kongming-quan"
  || me->query_skill_mapped("cuff") != "kongming-quan"
  || me->query_skill_mapped("cuff") != "kongming-quan" )
    return notify_fail("你现在无法使用「空字诀」。\n");	
	
	if (lv=me->query_skill("kongming-quan", 1) < 100)
		return notify_fail("你的空明拳尚不纯熟，还不能使用「空字诀」。\n");        
	
	if (me->query("neili") < 1000)
		return notify_fail("你现在内力不足，不能使用「空字诀」！\n");
  
  if (!me->query("quest/jiebai/pass"))
    return notify_fail("你没有和周伯通结义，不能使用「空字诀」。\n");
	
	if (me->query_temp("kongming/kong"))
		return notify_fail("你正在使用「空字诀」！\n");          
	
	message_vision(HIW"\n$N长啸一声，拳法陡然加快，双手拳招源源而出。\n"NOR, me);
	
	me->add("neili", -300 -lv/2); 
	me->start_perform(4,"空字诀");
	me->set_temp("kongming/kong", 1);
	me->add_temp("apply/force",300+lv/5);
  me->add_temp("apply/pfm_force",200+lv/5);
  me->add_temp("apply/pfm_attack",80+lv/10);
  me->add_temp("apply/action/color",HIW);
	call_out("check_fight", 1, me, lv, 10+me->query_skill("kongming-quan",1)/10);
	return 1;
}

void remove_effect(object me,int lv);
void check_fight(object me, int lvl, int count){
	if (!me) return;
	if( me->query_temp("weapon")
	|| me->query_skill_prepared("cuff") != "kongming-quan"
  || me->query_skill_mapped("cuff") != "kongming-quan"
  || me->query_skill_mapped("cuff") != "kongming-quan"
	|| !me->is_fighting()
	|| count < 1 ){
		remove_effect(me, lvl);
		return;
	}
	call_out("check_fight", 1, me, lvl, count-1);
}

void remove_effect(object me,int lv)
{
	if (objectp(me)) 
	{
		me->delete_temp("kongming/kong");
		me->delete_temp("apply/action/color");
		me->add_temp("apply/force",-300-lv/5);
	  me->add_temp("apply/pfm_force",-200-lv/5);
	  me->add_temp("apply/pfm_attack",-80-lv/10);
		tell_object(me, HIW"\n你拳法一窒，出手不由得慢了下来。\n"NOR);
	}
}

string perform_name(){ return HIW"空字诀"NOR; }

int help(object me)
{
	write(HIW"\n空明拳之「空字诀」："NOR"\n\n");
	write(@HELP
	要求：	
		当前内力 1000 以上；
		空明拳等级 100 以上；
		激发拳法为空明拳；
		激发招架为空明拳；
		备拳法为空明拳且无兵器。

HELP
	);
	return 1;
}