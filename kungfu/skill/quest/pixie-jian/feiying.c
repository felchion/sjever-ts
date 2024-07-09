// feiying.c 葵花飞影术

#include <ansi.h>

inherit F_SSERVER;

void remove_effect(object me,int lv);
string perform_name(){ return MAG"葵花飞影术"NOR; }
int perform(object me, object target)
{
	int skill,lv;
  object weapon ;
	
	if( !target ) target = me;
	if(!me->is_fighting())
		return notify_fail("葵花飞影术只能在战斗中使用。\n");


	if( (int)me->query("neili") < 2000  )
		return notify_fail("你的内力不够。\n");
	if( (int)me->query("jingli") < 1000  )
		return notify_fail("你的精力不够。\n");
	if( (int)me->query_temp("feiying") )
		return notify_fail("你已经在运用葵花飞影术了。\n");
	if( (int)me->query_skill("pixie-jian", 1) < 200 )
		return notify_fail("你的辟邪剑法还没练成，无法使用葵花飞影术。\n");
  if((string)me->query_skill_mapped("sword") != "pixie-jian" )
    return notify_fail("你现在无法使用「葵花飞影术」！\n");
  if (!objectp(weapon = me->query_temp("weapon"))
  || (string)weapon->query("skill_type") != "sword")
    return notify_fail("你使用的武器不对。\n");
  
    
	skill = me->query_skill("dodge") / 3;
	skill = skill + me->query_skill("pixie-jian");
	skill = skill / 5;
	lv = me->query_skill("pixie-jian",1);
	
	me->add("neili", -200-lv/2);
	me->add("jingli", -150-lv/4);


	message_vision(
	MAG "只见$N眼中青光一闪，凭空飞升离地尺余，身形飘浮不定，就如影子一般。\n" NOR, me);
	tell_object(me, MAG "你默运葵花飞影术，内劲外泄，将身体凭空抬了起来。\n" NOR);

  me->add_temp("apply/damage",300+lv/2);
  me->add_temp("apply/pfm_damage",200+lv/5);
  me->add_temp("apply/pfm_attack",120+lv/10);
  me->add_temp("apply/pfm_dodge", 100+lv/10);
  me->add_temp("apply/pfm_parry", 140+lv/10);
	me->set_temp("feiying", 1);

	call_out("check_fight", 1, me, lv, skill);

	if( me->is_fighting())
		me->start_busy(1);

	me->start_perform(skill,"「葵花飞影术」");
	return 1;
}


void check_fight(object me, int lvl, int count){
	object weapon;
	if (!me) return;
	if( !me->query_temp("weapon")
	|| (string)me->query_skill_mapped("sword") != "pixie-jian"
  || !objectp(weapon = me->query_temp("weapon"))
  || (string)weapon->query("skill_type") != "sword"
	|| !me->is_fighting()
	|| count < 1 ){
		remove_effect(me, lvl);
		return;
	}
	call_out("check_fight", 1, me, lvl, count-1);
}


void remove_effect(object me,int lv)
{
	if(!me) return;
  me->add_temp("apply/damage",-300-lv/2);
  me->add_temp("apply/pfm_damage",-200-lv/5);
  me->add_temp("apply/pfm_attack",-120-lv/10);
  me->add_temp("apply/pfm_dodge", -100-lv/10);
  me->add_temp("apply/pfm_parry", -140-lv/10);
  me->delete_temp("feiying");
  me->start_perform(0,"「葵花飞影术」");
  tell_object(me, MAG "你一遍葵花飞影术已经使完，身法又恢复正常。\n" NOR);
}

int help(object me)
{
	write(HIW"\n辟邪剑法之「葵花飞影术」："NOR"\n\n");
	write(@HELP
	
	要求：
		当前精力 1000 以上；
		当前内力 2000 以上；
		辟邪剑法等级 200 以上；
HELP
	);
	return 1;
}