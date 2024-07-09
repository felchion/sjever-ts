// an.c 太极拳「按字诀」诀
// by snowman@SJ 06/06/2000

inherit F_SSERVER;

#include <ansi.h>

int query_ap() {return 3;}

string perform_name(){ return HIR"按字诀"NOR; }

int perform(object me, object target)
{ 
	int lvl;
	if( !target ) target = offensive_target(me);

	if( !objectp(target) 
	|| !target->is_character() 
	|| !me->is_fighting(target) 
	|| !living(target))
		return notify_fail("「按字诀」只能对战斗中的对手使用。\n");

	if( userp(me) 
	&&( me->query_skill_mapped("parry") != "taiji-quan"
	&& me->query_skill_mapped("parry") != "taiji-jian") )
		return notify_fail("「按字诀」必须在用太极拳或太极剑激发招架的情况下才能使用。\n");

	if( userp(me) && (me->query_skill_mapped("force") != "yinyun-ziqi"
	|| me->query_skill_prepared("cuff") != "taiji-quan") )
		return notify_fail("你的内功或拳法不对，根本不能发挥出「按字诀」的优势。\n");  

	if( (int)me->query("neili", 1) < 500 )
		return notify_fail("你现在真气不够，强行使用「按字诀」是发挥不了它的威力的！\n");                   

	if( me->query_temp("weapon"))
		return notify_fail("你先放下手中的武器再说吧？！\n");     

  lvl = me->query_skill("taiji-quan",1);
	
	me->set_temp("apply/action/pfm", HIB"\n$N双臂陡然圈转，无声无息的运起太极拳中"RED"「按字诀」"HIB"，双臂聚力向$n按去"NOR);
  
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_hurt",350+lvl/2);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	COMBAT_D->do_attack(me, target,0,random(2)?0:3,2);
	me->add_temp("apply/pfm_hurt",-350-lvl/2);
	me->add_temp("apply/pfm_attack",-180-lvl/10);	      

	me->start_busy(random(2));
	return 1;      


}


int help(object me)
{
	write(WHT"\n太极「按字诀」："NOR"\n");
	write(@HELP
	太极拳虽变化万端，而理为一贯。由招熟而渐悟懂劲，由懂劲而阶及神明。
	「按字诀」的要点在于压敌，制敌用困，而后以泰山压顶之势胜出。此招集
	神、气、精、力、巧为一体，乃太极拳之精华！
	
	要求：	内力 1500 以上；      
		精力 300 以上；  
		太极拳等级 150 以上；    
HELP
	);
	return 1;
}
