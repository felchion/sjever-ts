//fogang.c 佛光普照
// by snowman@SJ

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 3;}

int perform(object me, object target)
{
	int i,damage;
	string msg;
 
	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target)
	 || !living(target))
		return notify_fail("「佛光普照」只能在战斗中使用。\n");

	if(objectp(me->query_temp("weapon")) )
		return notify_fail("你必须空手才能使用「佛光普照」！\n");

	if( me->query_skill("linji-zhuang", 1) < 150 && !wizardp(me))
		return notify_fail("你的临济十二庄未成，不能使用「佛光普照」！\n");

	if((int)me->query("max_neili") < 2000 )
		return notify_fail("你的内力修为不足，不能使用「佛光普照」！\n"); 

	if((int)me->query("neili") < 1000 )
		return notify_fail("你现在内力不足，不能使用「佛光普照」！\n"); 

	if((int)me->query_skill("jieshou-jiushi", 1) < 150 )
		return notify_fail("你的截手九式不够娴熟，不能使用「佛光普照」。\n");

	if(me->query_skill_prepared("hand") != "jieshou-jiushi"
	|| me->query_skill_mapped("hand") != "jieshou-jiushi"
	|| me->query_skill_mapped("parry") != "jieshou-jiushi")
		return notify_fail("你必须全面运用截手九式，才能使用「佛光普照」。\n");

	if(me->query_skill_mapped("force") != "linji-zhuang" && !wizardp(me))
		return notify_fail("你不使用临济十二庄，怎么能使用「佛光普照」？\n");

	if(me->query_temp("huanying")) 
		return notify_fail("你正在使用无形幻影，不能再使用「佛光普照」。\n");

	if(me->query_temp("shuangdao_hebi")) 
		return notify_fail("你正在使用「双刀合璧」，无法使用「佛光普照」。\n");

	msg = MAG "\n只见$N面显庄严，突然间全身骨骼中发出劈劈拍拍的轻微爆裂之声，炒豆般的\n"+
							"响声未绝，一式「"HIY"佛光普照"MAG"」，提一口真气，飘身而上，半空中手掌交错，一\n"+
							"股内劲狭带着热浪，如风如火，似雷似电，这一招拍出，招式平平淡淡，一成\n"+
						  "不变，击向$n，但却挡无可挡，避无可避";


  
  i  = me->query_skill("jieshou-jiushi",1);
  
  me->add("neili", -500-i/2);	
	me->add_temp("apply/damage",500+i/2);
	me->add_temp("apply/pfm_damage",120+i/5);
	me->add_temp("apply/pfm_attack",170+i/10);
	me->add_temp("apply/action/pfm",msg);
	
	me->add_temp("apply/pfm_damage",80);
	damage = COMBAT_D->do_attack(me, target, 0, random(2)?3:1);
	me->add_temp("apply/pfm_damage",-80);
	
	if(damage<1){ 
		msg = MAG"\n$N见一击不中，左手在空中划了一道弧线，右手顺手朝$n速攻了几招！\n"NOR;
		message_vision(msg, me, target);
		COMBAT_D->do_attack(me, target, 0, 1);
		if ( target && me->is_fighting(target))
		  COMBAT_D->do_attack(me, target, 0, random(2)?1:3);
	}
	
	me->add_temp("apply/damage",-500-i/2);
	me->add_temp("apply/pfm_damage",-120-i/5);
	me->add_temp("apply/pfm_attack",-170-i/10);
	me->delete_temp("apply/action/pfm");
	me->start_busy(random(2));
	return 1;
}

string perform_name(){ return MAG"佛光普照"NOR; }

int help(object me)
{
	write(MAG"\n截手九式之「佛光普照」："NOR"\n\n");
	write(@HELP
	要求：	当前内力 1000 以上；
		最大内力 2000 以上；
		截手九式等级 150 以上；
		临济十二庄等级 150 以上；
		激发手法为截手九式；
		激发招架为截手九式；
		空手且备截手九式。

HELP
        );
        return 1;
}
