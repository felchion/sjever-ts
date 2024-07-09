// tianyin.c 密宗大手印之不动明王根本印
#include <ansi.h>

inherit F_SSERVER;
int query_ap() {return 3;}
string perform_name(){ return HIW"不动明王根本印"NOR; }

int perform(object me, object target)
{
	int lvl;

	if( !target ) target = offensive_target(me);

	if( !target || !target->is_character() || !me->is_fighting(target) )
	  return notify_fail("「不动明王根本印」只能对战斗中的对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
	  return notify_fail("你必须空手才能使用「不动明王根本印」！\n");
	
	if (me->query_skill_prepared("hand") != "dashou-yin"
	|| me->query_skill_mapped("hand") != "dashou-yin")
		return notify_fail("你现在无法使用「不动明王根本印」。\n");
		
	if((int)me->query_skill("longxiang-boruo",1)<120)
	  return notify_fail("你的龙象般若功修为不够。\n");
	
	if( (int)me->query_skill("dashou-yin", 1) < 120 )
	  return notify_fail("你的密宗大手印修为不够，不能使用「不动明王根本印」。\n");
	
	if((int)me->query("max_neili",1)<1000)
	  return notify_fail("你的内力修为太差无法使用本势。\n");
	
	if( (int)me->query("neili", 1) < 500 )
	  return notify_fail("你现在内力太弱，不能使用「不动明王根本印」。\n");


  lvl = me->query_skill("dashou-yin",1);
	
	me->set_temp("apply/action/pfm", HIY"\n$N口诵金刚萨埵心咒，将双手食、中二指伸出，右手握于左手手心，正是一式"HIW"「不动明王根本印」"NOR);
  
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_hurt",300+lvl/2);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	COMBAT_D->do_attack(me, target,0,random(2)?0:3,2);
	me->add_temp("apply/pfm_hurt",-300-lvl/2);
	me->add_temp("apply/pfm_attack",-180-lvl/10);	      

	me->start_busy(random(2));
	return 1;      
}

int help(object me)
{
	write(HIW"\n大手印之「不动明王根本印」："NOR"\n");
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