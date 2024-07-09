// tianyin.c 密宗大手印之宝瓶印
#include <ansi.h>

inherit F_SSERVER;

string perform_name(){ return HIC"宝瓶印"NOR; }
int query_ap() {return 3;}

int perform(object me, object target)
{
	int lvl;

	if( !target ) target = offensive_target(me);

	if( !target || !target->is_character() || !me->is_fighting(target) )
	  return notify_fail("「宝瓶印」只能对战斗中的对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
	  return notify_fail("你必须空手才能使用「宝瓶印」！\n");
	
	if (me->query_skill_prepared("hand") != "dashou-yin"
	|| me->query_skill_mapped("hand") != "dashou-yin")
		return notify_fail("你现在无法使用「宝瓶印」。\n");
		
	if((int)me->query_skill("longxiang-boruo",1)<120)
	  return notify_fail("你的龙象般若功修为不够。\n");
	
	if( (int)me->query_skill("dashou-yin", 1) < 120 )
	  return notify_fail("你的密宗大手印修为不够，不能使用「宝瓶印」。\n");
	
	if((int)me->query("max_neili",1)<1000)
	  return notify_fail("你的内力修为太差无法使用本势。\n");
	
	if( (int)me->query("neili", 1) < 500 )
	  return notify_fail("你现在内力太弱，不能使用「宝瓶印」。\n");


  lvl = me->query_skill("dashou-yin",1);
	
	me->set_temp("apply/action/pfm", HIY"\n$N口诵摩利支天心咒，虚合双掌食指相背指尖微屈，复以拇指轻按食指前端作弹指状，正是一式"HIC"「宝瓶印」"NOR);
  
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/force",300+lvl);
	me->add_temp("apply/pfm_force",150+lvl/5);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	COMBAT_D->do_attack(me, target,0,random(2)?0:3);
	me->add_temp("apply/force",-300-lvl);
	me->add_temp("apply/pfm_force",-150-lvl/5);
	me->add_temp("apply/pfm_attack",-180-lvl/10);	      
	me->delete_temp("apply/action/");
     

	me->start_busy(random(2));
	return 1;      
}

int help(object me)
{
	write(HIC"\n大手印之「宝瓶印」："NOR"\n");
	write(@HELP

	
  要求：最大内力 1000 以上；      
        当前内力  500 以上；  
        龙象般若功等级 120 以上； 
        密宗大手印等级 120 以上；    
        手中无兵器。
HELP
	);
	return 1;
}