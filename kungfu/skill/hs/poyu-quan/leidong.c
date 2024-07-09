// kong.c  
// by tiantian@sj 2000.6.19

#include <ansi.h>

inherit F_SSERVER;
int query_ap() {return 3;}

int perform(object me,object target)
{
	int i,lv;
	
	if (!target) target = offensive_target(me);
	
	if (!target || !me->is_fighting(target))
		return notify_fail("「雷动九天」只能在战斗中使用。\n");                

	if( me->query_temp("weapon") )
    return notify_fail("你必须空手使用「雷动九天」！\n");
    
	if (me->query_skill_prepared("cuff") != "poyu-quan"
  || me->query_skill_mapped("cuff") != "poyu-quan"
  || me->query_skill_mapped("cuff") != "poyu-quan" )
    return notify_fail("你现在无法使用「雷动九天」。\n");	
	
	if (lv=me->query_skill("poyu-quan", 1) < 100)
		return notify_fail("你的劈石破玉拳尚不纯熟，还不能使用「雷动九天」。\n");        
	
	if (me->query("neili") < 1000)
		return notify_fail("你现在内力不足，不能使用「雷动九天」！\n");
  
	
	if (me->query_temp("poyu/leidong"))
		return notify_fail("你正在使用「雷动九天」！\n");          
	
	message_vision(HIY"\n$N一声轻喝，双手紫气萦绕，出拳如九天惊雷，$n顿时觉得压力大增！\n"NOR, me,target);
	
	me->add("neili", -500 -lv/2); 
	me->set_temp("poyu/leidong", 1);
	me->add_temp("apply/force",300+lv/2);
  me->add_temp("apply/pfm_force",200+lv/5);
  me->add_temp("apply/pfm_attack",120+lv/10);
  me->add_temp("apply/action/color",HIY);
	
	
	for(i = 0; i < 3; i++)
    if (objectp(target) && me->is_fighting(target))
      COMBAT_D->do_attack(me, target, 0, random(3)?0:3);
	
	call_out("check_fight", 1, me, lv, 20);
	return 1;
}

void remove_effect(object me,int lv);
void check_fight(object me, int lvl, int count){
	if (!me) return;
	if( me->query_temp("weapon")
	|| me->query_skill_prepared("cuff") != "poyu-quan"
  || me->query_skill_mapped("cuff") != "poyu-quan"
  || me->query_skill_mapped("cuff") != "poyu-quan"
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
		me->delete_temp("poyu/leidong");
		me->delete_temp("apply/action/color");
		me->add_temp("apply/force",-300-lv/2);
	  me->add_temp("apply/pfm_force",-200-lv/5);
	  me->add_temp("apply/pfm_attack",-120-lv/10);
		tell_object(me, CYN"\n你心念一动，拳势顿时缓了下来。\n"NOR);
	}
}

string perform_name(){ return HIY"雷动九天"NOR; }

int help(object me)
{
	write(HIY"\n劈石破玉拳之「雷动九天」："NOR"\n\n");
	write(@HELP
	要求：	
		当前内力 1000 以上；
		劈石破玉拳等级 100 以上；
		激发拳法为劈石破玉拳；
		激发招架为劈石破玉拳；
		备拳法为劈石破玉拳且无兵器。

HELP
	);
	return 1;
}