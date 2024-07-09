//By lsxk@hsbbs 2007/10/19
//群魔乱舞

#include <ansi.h>

inherit F_SSERVER;


string perform_name(){ return HIB"群魔"HIR"乱舞"NOR; }
int query_ap() {return 15;}

int perform(object me,object target)
{
	object weapon;
	int i,j,lv,hit;

	if( !target ) target = offensive_target(me);

	if( !objectp (target) || !me->is_fighting(target) || !living(target)
	|| environment(target)!=environment(me))
		return notify_fail("群魔乱舞只能对战斗中的对手使用。\n");

	if(me->query_skill_mapped("force") != "huagong-dafa")
		return notify_fail("你的内功不是化功大发，无法使用群魔乱舞！\n");
	if( (int)me->query_skill("huagong-dafa", 1) < 250 )
		return notify_fail("你的内功还未练成，不能使用群魔乱舞！\n");
	if( (lv=(int)me->query_skill("tianshan-zhang", 1)) < 250 )
		return notify_fail("你的天山杖法还未练成，不能使用群魔乱舞！\n");
	if((int)me->query_skill("staff", 1) < 250 )
		return notify_fail("你的基本杖法不够娴熟，不能在使用天下无狗。\n");
	if((int)me->query_skill("poison", 1) < 180 )
		return notify_fail("你的毒技不够娴熟，不能在使用群魔乱舞。\n");

	if (!objectp(weapon = me->query_temp("weapon")) || weapon->query("skill_type") != "staff"
	|| me->query_skill_mapped("staff") != "tianshan-zhang"
	|| me->query_skill_mapped("parry") != "tianshan-zhang")
		return notify_fail("你现在使得了群魔乱舞么？\n");

	if((int)me->query("max_neili") < 3000 )
		return notify_fail("你的内力修为不够，不能使用群魔乱舞！\n");
	if((int)me->query("neili") < 2400 )
		return notify_fail("你现在真气不足，不能使用群魔乱舞！\n");

	message_vision(HIB"\n$N突然一声怪叫，脚步飘忽不定，瞬间已然向$n挥出数招，气势吓人之极！\n"NOR,me,target);

	hit = (lv/100)+2;
	if(hit > 7) hit = 8;

	me->receive_damage("neili", hit*300);
	me->add_temp("apply/pfm_attack", 180+lv/5);
	me->add_temp("apply/damage", 800+lv*2);
	me->add_temp("apply/pfm_damage", 180+lv/4);

	for( i=0; i < hit; i++ ){
		for(j=0;j<4;j++)
		if(!target || target->query("qi") < 0) target = offensive_target(me);
		if (target && me->is_fighting(target) && target->query("qi")>0)
			COMBAT_D->do_attack(me, target,weapon,random(2)?0:3);
	}
	me->add_temp("apply/pfm_attack", -180-lv/5);
	me->add_temp("apply/damage", -800-lv*2);
	me->add_temp("apply/pfm_damage", -180-lv/4);

	me->start_busy(1);
	return 1;
}

int help(object me)
{
   write(WHT"\n天山杖法「"HIB"群魔"HIR"乱舞"WHT"」："NOR"\n");
   write(@HELP

   要求：  最大内力 4500 以上；
           内力 2000 以上；
           天山杖法等级 250 以上；
           化功大法等级 250 以上；
           毒技等级 180 以上；
           激发招架为天山杖法。

HELP
   );
   return 1;
}
