// wujian.c
// tiantian@sj
// modified by olives@SJ 4/31/2001
#include <ansi.h>
#include <combat.h>
#include <skill_class.h>
inherit F_SSERVER;

string perform_name() {return HIB"无剑胜有剑"NOR;}
int query_ap() {return 7;}

void remove_effect(object me, object weapon);
int perform(object me, object target)
{

	int i;
	object weapon;           

	if( !target ) target = offensive_target(me);

	if( !target || !me->is_fighting(target) )
		return notify_fail("「无剑胜有剑」只能在战斗中使用。\n");

	if(me->query_temp("wujian"))
		return notify_fail("你正在使用「无剑胜有剑」。\n");

	if(!objectp(weapon = me->query_temp("weapon")) 
	|| weapon->query("skill_type")!="sword")
		return notify_fail("你手中无剑，如何使的出玄铁剑法的绝技！\n");

	if( (i=(int)me->query_skill("xuantie-jianfa", 1)) < 450 ) 
		return notify_fail("你的玄铁剑法不够娴熟，不会使用「无剑胜有剑」。\n");
		
	if( (int)me->query_skill("yunu-xinjing", 1) < 450 )
		return notify_fail("你的基本内功等级不够，不能使用「无剑胜有剑」。\n");

  if (me->query_skill_mapped("sword") != "xuantie-jianfa" )
		return notify_fail("你现在无法使用「无剑胜有剑」。\n");

  if( me->query_skill_mapped("force") != "yunu-xinjing" && userp(me))
   return notify_fail("你现在无法使用「无剑胜有剑」！\n");
	
	if( me->query("max_neili")<5000)
		return notify_fail("你的内力修为太差，不能使用「无剑胜有剑」。\n");
	
	if( (int)me->query("neili") < 3000 )
		return notify_fail("你现在内力太弱，不能使用「无剑胜有剑」。\n");

	message_vision(HIB "\n$N将内力灌注剑上，纯以意气运剑，但觉天地间无一不可为剑，渐入「无剑胜有剑」的境界。\n" NOR, me, target);
  
	me->add("neili",-i*3);
	me->add_temp("apply/pfm_attack", 240);
	me->add_temp("apply/pfm_damage", 240);
	me->add_temp("apply/damage", 800);
	me->set_temp("apply/action/class",KONG);
	me->set_temp("wujian",1);

	call_out("check_fight",10,me,weapon,i/50);
	return 1;
}

void check_fight(object me, object weapon,int count) 
{ 
	if(!me
	|| !living(me)
	||  me->is_ghost()
	|| !me->is_fighting()
	|| !me->query_temp("weapon")
	|| me->query_temp("weapon") != weapon 
	|| me->query_skill_mapped("sword") != "xuantie-jianfa" 
	|| count < 1 ){
		remove_effect(me,weapon);
		return;
	}
  call_out("check_fight", 10, me, weapon,count-1); 
} 

void remove_effect(object me, object weapon)
{
	if (!me) return;
	me->add_temp("apply/pfm_attack", -240);
	me->add_temp("apply/pfm_damage", -240);
	me->add_temp("apply/damage", -800);
	me->delete_temp("apply/action/class");
	me->delete_temp("wujian");
}


int help(object me)
{
        write(HIB"\n「无剑胜有剑」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 3000 以上；
                最大内力 5000 以上；
                玄铁剑法等级 450 以上；
                玉女心经等级 450 以上；
                激发剑法为玄铁剑法；
                激发招架为玄铁剑法。

HELP
        );
        return 1;
}