// diezhang.c (昆仑掌法)  perform 昆仑叠掌
// cck 21/8/97
// cool 981010
// augx@sj 8/28/2001
#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 3;}

int perform(object me, object target)
{
  int lvl;
	
	if( !target )
	target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) )
	return notify_fail("「昆仑叠掌」只能在战斗中对对手使用。\n");

	if( objectp( me->query_temp("weapon")) )
	return notify_fail("只有空手才能施展「昆仑叠掌」。\n");

	if( me->query_skill("strike") < 120 )
	return notify_fail("你的掌法不够娴熟，不会使用「昆仑叠掌」。\n");

	if( me->query_skill("xuantian-wuji", 1) < 120 )
	return notify_fail("你的玄天无极功等级不够，不能使用「昆仑叠掌」。\n");
	
	if( lvl=me->query_skill("kunlun-zhang", 1) < 120 )
	return notify_fail("你的玄天无极功等级不够，不能使用「昆仑叠掌」。\n");

	if( me->query("max_neili") < 1500 )
	return notify_fail("你内力修为不够，不能使用「昆仑叠掌」。\n");

	if( me->query("neili") < 1000 )
	return notify_fail("你现在内力太弱，不能使用「昆仑叠掌」。\n");

	if( me->query("jingli") < 100 )
	return notify_fail("你现在精力不够，自己小心啊！\n");

	if (me->query_skill_prepared("strike") != "kunlun-zhang"
	|| me->query_skill_mapped("force") != "xuantian-wuji"
	|| me->query_skill_mapped("strike") != "kunlun-zhang"
	|| me->query_skill_mapped("parry") != "kunlun-zhang" )
	return notify_fail("你现在无法使用「昆仑叠掌」进行攻击。\n");

	if( me->query("env/昆仑叠掌") == "连掌叠合" ) {
		message_vision(HIG"\n$N深吸一口气，伴随大喝之声一掌击出，使的正是「昆仑叠掌」掌法。"NOR, me, target);


		me->receive_damage("neili", 500 + lvl/2);

		me->add_temp("apply/pfm_force", 150 + lvl/5);
		me->add_temp("apply/force", 500 + lvl);
		me->add_temp("apply/pfm_attack", 180+lvl/10);


		COMBAT_D->do_attack(me, target, 0, random(2)?1:3);

	  if (me->is_fighting(target))
			COMBAT_D->do_attack(me, target, 0, random(2)?1:3);


		me->add_temp("apply/pfm_force", -150 - lvl/5);
		me->add_temp("apply/force", -500 - lvl);
		me->add_temp("apply/pfm_attack", -180 - lvl/10);


		me->add_busy(1);
		return 1;
	}

	// 掌力重叠 的化学 pfm

	me->set_temp("apply/action/pfm", HIG"$N深吸一口气，大喝一声，连续击出两掌，后招推着前招，掌力叠合在一起扑向$n"NOR);

	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_hurt",500+lvl/2);
	me->add_temp("apply/pfm_attack",240+lvl/10);
	COMBAT_D->do_attack(me, target,0,random(2)?0:3,2);	
	me->add_temp("apply/pfm_hurt",-500-lvl/2);
	me->add_temp("apply/pfm_attack",-240-lvl/10);	    
	

	me->add_busy(1);

	return 1;
}

string perform_name(){ return HIG"昆仑叠掌"NOR; }
int help(object me)
{
	write(HIG"\n昆仑掌法之「昆仑叠掌」："NOR"\n\n");
	write(@HELP
	连续击出数掌，掌力叠加攻击敌人，显现昆仑武学以柔克刚的特色。
	要求：玄天无极功 120级，
	      昆仑掌法 120级，
	      基本掌法 120级,
	      当前内力 1000，
	      最大内力 1500,


	注：不同情况下，可以通过 set 产生不同的效果
		set 昆仑叠掌 连掌叠合
		set 昆仑叠掌 掌力重叠

HELP
	);
	return 1;
}
