// leiting 雷霆万钧
// Created by Numa@Sj 19991118

#include <ansi.h>
#include <combat.h> 
inherit F_SSERVER;

int query_ap() {return 2;}

int perform(object me, object target)
{
	object weapon;
	int lvl;
	        
	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) )
		return notify_fail("「雷霆万钧」只能在战斗中使用。\n");
        
	if( (int)me->query_skill("songshan-jian", 1) < 160 )
		return notify_fail("你的嵩山剑法还不够娴熟，无法使出「雷霆万钧」。\n");

	if( (int)me->query_skill("hanbing-zhenqi", 1) < 160 )
		return notify_fail("你的寒冰真气等级还不够，无法使出「雷霆万钧」。\n");

	if (me->query_skill_mapped("force") != "hanbing-zhenqi")
		return notify_fail("你目前的内功无法使出「雷霆万钧」。\n");

	if (!objectp(weapon = me->query_temp("weapon"))
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "songshan-jian"
	|| me->query_skill_mapped("parry") != "songshan-jian" )
		return notify_fail("你无法使出「雷霆万钧」。\n");

	if (me->query_temp("ss/leiting"))
		return notify_fail("你刚使用完「雷霆万钧」，休息一下吧。\n"); 
                
	if( (int)me->query("max_neili") < 2500)
		return notify_fail("你的内力修为太差，无法使出「雷霆万钧」。\n");
                
	if( (int)me->query("neili") < 1000)
		return notify_fail("你现在真气太弱，使不出「雷霆万钧」。\n");
	
	lvl = me->query_skill("songshan-jian", 1);	
	
	me->set_temp("ssj/leiting",1);
			
	me->set_temp("apply/action/pfm", YEL"\n$N右手衣袖鼓了起来，犹似吃饱了风的帆篷一般，全身劲力都集中到了右臂之上，全力向着$n刺出"NOR);
  me->set_temp("apply/action/hit", HIR"$n被这招雷霆万钧当胸猛撞，立时口中鲜血狂喷而出！\n"NOR);       
	target->set_temp("apply/action/parry", CYN"突然之间，白影急晃，$n向后滑出丈余，立时又回到了原地。\n"NOR );
	target->set_temp("apply/action/dodge", CYN"突然之间，白影急晃，$n向后滑出丈余，立时又回到了原地。\n"NOR );	


	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/damage",500+lvl);
	me->add_temp("apply/pfm_damage",200+lvl/5);
	me->add_temp("apply/pfm_attack",150+lvl/10);
	COMBAT_D->do_attack(me, target,0,random(2)?0:3);
	me->add_temp("apply/damage",-500-lvl);
	me->add_temp("apply/pfm_damage",-200-lvl/5);
	me->add_temp("apply/pfm_attack",-150-lvl/10);  
	me->delete_temp("ssj/leiting",1);
	me->delete_temp("apply/action/");	
	target->delete_temp("apply/action/");
	

	me->start_busy(1);
  return 1;
}

string perform_name(){ return YEL"雷霆万钧"NOR; }

int help(object me)
{
        write(YEL"\n嵩山剑法之「雷霆万钧」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 2500 以上；
                当前精力 1000 以上；
                嵩山剑法等级 160 以上；
                寒冰真气等级 160 以上；
                有效内功等级 250 以上；
                激发剑法为嵩山剑法；
                激发招架为嵩山剑法；
                激发内功为寒冰真气；
                且手持兵器。

HELP
        );
        return 1;
}
