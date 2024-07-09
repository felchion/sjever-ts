// tianyin.c 密宗大手印之内缚印
#include <ansi.h>

inherit F_SSERVER;
int query_ap() {return 3;}
string perform_name(){ return BLU"内缚印"NOR; }

int perform(object me, object target)
{
	int lvl;

	if( !target ) target = offensive_target(me);

	if( !target || !target->is_character() || !me->is_fighting(target) )
	  return notify_fail("「内缚印」只能对战斗中的对手使用。\n");

	if(target->query_condition("no_exert"))
	  return notify_fail(target->query("name")+"已经是气息不匀！\n");
	
	if( objectp(me->query_temp("weapon")) )
	  return notify_fail("「内缚印」只能对战斗中的对手使用。\n");

	if (me->query_skill_prepared("hand") != "dashou-yin"
	|| me->query_skill_mapped("hand") != "dashou-yin")
		return notify_fail("你现在无法使用「内缚印」。\n");
			
	if((int)me->query_skill("longxiang-boruo",1)<200)
	  return notify_fail("你的龙象般若功修为不够。\n");
	
	if( (int)me->query_skill("dashou-yin", 1) < 200 )
	  return notify_fail("你的密宗大手印修为不够，不能使用「内缚印」。\n");
	
	if((int)me->query("max_neili",1)<3000)
	  return notify_fail("你的内力修为太差无法使用本势。\n");
	
	if( (int)me->query("neili", 1) < 1000 )
	  return notify_fail("你现在内力太弱，不能使用「内缚印」。\n");


  lvl = me->query_skill("dashou-yin",1);
	
	me->set_temp("apply/action/pfm", HIY"\n$N口诵莲花生大士六道金刚咒，十指交叉遂而均屈向掌中，正是一式"BLU"「内缚印」"NOR);
  me->set_temp("apply/action/hit", HIR"$p被印中胸口，全身内劲乱窜，无法自己！\n"NOR);       

	
	me->receive_damage("neili", 500+lvl/2);

	me->add_temp("apply/damage", 400+lvl/2);
	me->add_temp("apply/pfm_damage", 160+lvl/5);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	if (COMBAT_D->do_attack(me, target))
	  target->add_condition("no_exert", 6);
	me->add_temp("apply/damage", -400-lvl/2);
	me->add_temp("apply/pfm_damage", -160-lvl/5);
	me->add_temp("apply/pfm_attack",-180-lvl/10);    
	me->delete_temp("apply/action/"); 

	me->start_busy(2);
	return 1;      
}

int help(object me)
{
	write(BLU"\n大手印之「内缚印」："NOR"\n");
	write(@HELP

	
  要求：最大内力 3000 以上；      
        当前内力 1000 以上；  
        龙象般若功等级 200 以上； 
        密宗大手印等级 200 以上；    
        手中无兵器。
HELP
	);
	return 1;
}