// hama.c 蛤蟆功
// by snowman@SJ

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 2;}

int perform(object me, object target)
{
	int lvl;

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !target->is_character()
	|| !me->is_fighting(target) || !living(target) )
	return notify_fail("蛤蟆功绝招只能对战斗中的对手使用。\n");

	if( (lvl=(int)me->query_skill("hamagong", 1)) < 100 )
	return notify_fail("你的蛤蟆功还不够熟练，不能使用蛤蟆绝招对敌！\n");

	if( me->query_temp("weapon") )
	return notify_fail("你必须空手才能使用「蛤蟆功」！\n");

	if (me->query_skill_mapped("force") != "hamagong")
	return notify_fail("你的内功中无此功夫。\n");

	if (me->query_skill_prepared("strike") != "hamagong" ||
	me->query_skill_mapped("strike") != "hamagong")
	return notify_fail("你必须先将蛤蟆功运用于掌法之中才行。\n");

	if( (int)me->query("max_neili") < 1500 )
	return notify_fail("你现在内力不足，使不出蛤蟆功的绝招！\n");

	if( (int)me->query("neili") < 500 )
	return notify_fail("你现在内力不足，使不出蛤蟆功的绝招！\n");

	if( (int)me->query_skill("strike", 1) < 100 )
	return notify_fail("你的掌法不够娴熟，使不出蛤蟆功的绝招！\n");

	if (me->query_str(1) < 30 )
	return notify_fail("你的力量太小，使不出用蛤蟆绝招！\n");


	me->set_temp("hmg_hama", 1);

	me->receive_damage("neili", 500+lvl);
	me->receive_damage("jingli", 500);

	me->add_temp("apply/pfm_damage", 160+lvl/4);
	me->add_temp("apply/damage", 500+lvl);
	me->add_temp("apply/pfm_attack", 180+lvl/10);

  me->set_temp("apply/action/pfm", HBBLU"$N身子蹲下，左掌平推而出，使的正是$N生平最得意的「蛤蟆功」绝招，掌风直逼$n而去");
	target->set_temp("apply/action/dodge", HIY"$p但觉一股微风扑面而来，风势虽然不劲，然已逼得自己呼吸不畅，知道不妙，连忙跃开数尺。\n" NOR);
  target->set_temp("apply/action/parry", HIY"$p但觉一股微风扑面而来，风势虽然不劲，然已逼得自己呼吸不畅，知道不妙，连忙跃开数尺。\n" NOR);
	
	COMBAT_D->do_attack(me, target);
	
	if(me->is_fighting(target)) {
	  me->set_temp("apply/action/pfm", HBBLU"$N左掌劲力未消，右掌也跟着推出，功力相叠，「蛤蟆功」掌风排山倒海般涌向$n");
	  target->set_temp("apply/action/dodge", HIY"$p喘息未定，又觉一股劲风扑面而来，连忙跃开数尺，狼狈地避开。\n" NOR);
    target->set_temp("apply/action/parry",HIY"$p喘息未定，又觉一股劲风扑面而来，连忙跃开数尺，狼狈地避开。\n" NOR);
    COMBAT_D->do_attack(me, target);
  }
	
	if(me->is_fighting(target)) {
	  me->set_temp("apply/action/pfm", HBRED "\n$N双腿一登，双掌相并向前猛力推出，$n连同身前方圆三丈全在「蛤蟆功」劲力笼罩之下");
	  target->set_temp("apply/action/dodge", HIY"$p用尽全身力量向右一纵一滚，摇摇欲倒地站了起来，但总算躲开了这致命的一击！\n"NOR);
    target->set_temp("apply/action/parry", HIY"$p用尽全身力量向右一纵一滚，摇摇欲倒地站了起来，但总算躲开了这致命的一击！\n"NOR);
    COMBAT_D->do_attack(me, target,0,3);
  }
	me->add_temp("apply/pfm_damage", -160-lvl/4);
	me->add_temp("apply/damage", -500-lvl);
	me->add_temp("apply/pfm_attack", -180-lvl/10);
	me->delete_temp("apply/action");
	target->delete_temp("apply/action");

	return 1;
}

 string perform_name(){ return BLU"蛤蟆功"NOR; }

int help(object me)
{
	write(WHT"\n蛤蟆功「蛤蟆功」："NOR"\n");
	write(@HELP
	欧阳锋的独门绝技蛤蟆功乃是天下武学中的绝顶功夫。

	要求：	内力 500 以上；
		最大内力 1500 以上； 
		蛤蟆功等级 100 以上；
		基本掌法等级 100 以上；
		空手备掌法为蛤蟆功。

HELP
	);
	return 1;
}
