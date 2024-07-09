// wuji.c 无级
#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return HIR"混元无极"NOR;}
int query_ap() {return 3;}

int perform(object me, object target)
{

	int lvl;
	lvl = ((int)me->query_skill("hunyuan-zhang",1));
	if( !target ) target = offensive_target(me);

	if(!objectp(target) || !target->is_character() || !me->is_fighting(target) )
	return notify_fail("混元无极只能对战斗中的对手使用。\n");

	if( (int)me->query_skill("hunyuan-zhang", 1) < 100 )
	return notify_fail("你的混元掌不够娴熟，不会使用混元无极！\n");

	if(me->query_skill_mapped("force") != "huashan-qigong"
	&& me->query_skill_mapped("force") != "zixia-gong" )
	return notify_fail("你的特殊内功不对，不能用出混元无极伤敌。\n");
	
	if (me->query_skill_prepared("strike") != "hunyuan-zhang"
	|| me->query_skill_mapped("strike") != "hunyuan-zhang")
		return notify_fail("你现在无法使用「混元无极」！\n");

	if( (int)me->query("neili") < 800 )
	return notify_fail("你现在内力太弱，无法使出混元无极。\n");

	me->set_temp("apply/action/pfm", HIR "$N暗运紫霞神功，脸上返着暗暗的紫色，一股混元掌柔和的掌力想$n逼去");
  me->set_temp("apply/action/hit", RED"结果$N一击命中，$n只感到浑身一股暖流向全身散去，突然感到一种难以忍受的灼热。\n"NOR);       
	target->set_temp("apply/action/parry", CYN"可是$p看破了$P的招数，向旁跳开数步，躲开了$P的凌厉一击！\n"NOR);
	target->set_temp("apply/action/dodge", CYN"可是$p看破了$P的招数，向旁跳开数步，躲开了$P的凌厉一击！\n"NOR);	

	me->receive_damage("neili", 500/2+lvl/2);
	me->add_temp("apply/force",500+lvl);
	me->add_temp("apply/pfm_force",150+lvl/4);
	me->add_temp("apply/pfm_attack",150+lvl/10);
	COMBAT_D->do_attack(me, target,0,random(2)?0:3);
	me->add_temp("apply/force",-500-lvl);
	me->add_temp("apply/pfm_force",-150-lvl/4);
	me->add_temp("apply/pfm_attack",-150-lvl/10);	      
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/");	
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
	write(HIR"\n混元掌之「混元无极」："NOR"\n");
	write(@HELP
	要求： 混元掌等级 100 以上；      
		     当前内力 800 以上；      

HELP
	);
	return 1;
}