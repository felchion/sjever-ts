// 调整 by lane 2004.12.13 
#include <ansi.h>
#include <combat.h> 

inherit F_SSERVER;
int query_ap() {return 6;}

int perform(object me, object target)
{
	string msg;
	int i,lvl;
	object weapon;
	lvl = me->query_skill("cihang-bian", 1);

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target) )
		return notify_fail("「慈悲字诀」只能对战斗中的对手使用。\n");

	if( lvl < 160 )
		return notify_fail("你的慈行鞭法修为太差,还不能使用「慈悲字诀」！\n");

	if( me->query_temp("chb") )
		return notify_fail("你出现问题，现在无法使用「慈悲字诀」。\n");

	if( me->query_skill("kurong-changong", 1) < 160 )
		return notify_fail("你的枯荣禅功修为太差，还不能使用「慈悲字诀」！\n");

	if( me->query("max_neili") < 2000 )
		return notify_fail("你的内力修为不够辅助慈悲字诀。\n");

	if( me->query("neili") < 2000 )
		return notify_fail("你现在内力不足，不够辅助慈悲字诀。\n");

	if( me->query("jingli") < 1200 )
		return notify_fail("你的精力修为不够辅助慈悲字诀。\n");

	weapon = me->query_temp("weapon"); 

	if( me->query_skill_mapped("whip") != "cihang-bian" && userp(me) )
		return notify_fail("你现在无法使用「慈悲诀」！\n");

	if( me->query_skill_mapped("parry") != "cihang-bian" && userp(me) )
		return notify_fail("你现在无法使用「慈悲诀」！\n");

	if( !weapon || weapon->query("skill_type") != "whip" )
		return notify_fail("你手中没有兵器如何使用「慈悲诀」。\n");

	msg = HIY"\n只见$N喃喃自语道：慈悲为怀，手中的"+ weapon->name()+HIY"仿佛如来出世般倒卷向$n。\n"NOR;
	message_vision(msg, me, target);

	me->add("neili", -500-lvl);
	me->add("jingli", -150);

	me->set_temp("chb", 3);
  me->add_temp("apply/damage",500+lvl);
  me->add_temp("apply/pfm_damage",250+lvl/3);
  me->add_temp("apply/pfm_attack",240+lvl/10);
  me->set_temp("apply/action/color", WHT);

	for(i=0;i < 4; i++)
		if (objectp(target) && me->is_fighting(target))
			COMBAT_D->do_attack(me,target, me->query_temp("weapon"), i==3?3:0);

  me->add_temp("apply/damage",-500-lvl);
  me->add_temp("apply/pfm_damage",-250-lvl/3);
  me->add_temp("apply/pfm_attack",-240-lvl/10);
  me->delete_temp("apply/action/color");
	me->delete_temp("chb");
  me->start_busy(random(2)+1);
	
	return 1;
}

string perform_name(){ return HIY"慈悲诀"NOR; } 

int help(object me)
{
	write(HIY"\n慈航鞭之「慈悲诀」："NOR"\n\n");
	write(@HELP
   要求：  当前内力 1500 以上；
           当前精力 1200 以上；
           慈航鞭等级 160 以上；
           枯荣禅功等级 160 以上；
           禅宗心法等级 180 以上：
           激发鞭法为慈航鞭；
           激发招架为慈航鞭；
           必须出家且手持兵器。

HELP
        );
	return 1;
}
