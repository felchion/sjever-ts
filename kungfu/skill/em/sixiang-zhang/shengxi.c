#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 4;}

int perform(object me, object target)
{
	int i;
	string msg;
 
	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target)
	 || !living(target))
		return notify_fail("「生息不止」只能在战斗中使用。\n");

	if(objectp(me->query_temp("weapon")) )
		return notify_fail("你必须空手才能使用「生息不止」！\n");

	if( me->query_skill("linji-zhuang", 1) < 150 && !wizardp(me))
		return notify_fail("你的临济十二庄未成，不能使用「生息不止」！\n");

	if((int)me->query("max_neili") < 2000 )
		return notify_fail("你的内力修为不足，不能使用「生息不止」！\n"); 

	if((int)me->query("neili") < 1000 )
		return notify_fail("你现在内力不足，不能使用「生息不止」！\n"); 

	if((int)me->query_skill("sixiang-zhang", 1) < 150 )
		return notify_fail("你的四象掌不够娴熟，不能使用「生息不止」。\n");

	if(me->query_skill_prepared("strike") != "sixiang-zhang"
	|| me->query_skill_mapped("strike") != "sixiang-zhang"
	|| me->query_skill_mapped("parry") != "sixiang-zhang")
		return notify_fail("你必须全面运用四象掌，才能使用「生息不止」。\n");

	if(me->query_skill_mapped("force") != "linji-zhuang")
		return notify_fail("你不使用临济十二庄，怎么能使用「生息不止」？\n");

  msg = HIC"\n$N高高跃起凌空而立，双臂环恃如圆，掌间隐隐闪现四灵幻相，如龙、如虎、如鸟、如龟！\n"+
        HIW"$n哪里见过此等绝学，心神顿是一震。正值$n错愕之际，$N双掌平推，漫天掌力向$n席卷而去";
  
  i  = me->query_skill("sixiang-zhang",1);
  
  me->add("neili", -500-i/2);	
	me->add_temp("apply/damage",500+i);
	me->add_temp("apply/pfm_damage",150+i/4);
	me->add_temp("apply/pfm_attack",240+i/10);
	me->add_temp("apply/action/pfm",msg);

	COMBAT_D->do_attack(me, target, 0, random(2)?1:3); 	

	
	me->add_temp("apply/damage",-500-i);
	me->add_temp("apply/pfm_damage",-150-i/4);
	me->add_temp("apply/pfm_attack",-240-i/10);
	me->delete_temp("apply/action/pfm");
	me->start_busy(random(2));
	return 1;
}

string perform_name(){ return HIW"生息不止"NOR; }

int help(object me)
{
	write(MAG"\n四象掌之「生息不止」："NOR"\n\n");
	write(@HELP
	要求：	当前内力 1000 以上；
		最大内力 2000 以上；
		四象掌等级 150 以上；
		临济十二庄等级 150 以上；
		激发掌法为四象掌；
		激发招架为四象掌；
		空手且备四象掌。

HELP
        );
        return 1;
}
