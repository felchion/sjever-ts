#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 3;}

int perform(object me, object target)
{
	int lvl;
	object weapon;
	lvl = me->query_skill("xuanyin-jian", 1);

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !necessary(target) )
	return notify_fail("你只能对战斗中的对手使用玄阴诀。\n");

	if( me->query_skill("xuanyin-jian", 1) < 160 )
	return notify_fail("你的玄阴剑法不够娴熟，无法施展出玄阴诀。\n");

	weapon = me->query_temp("weapon");

	if ( ! weapon
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "xuanyin-jian"
	|| me->query_skill_mapped("parry") != "xuanyin-jian" )
	return notify_fail("你现在无法施展出玄阴诀。\n");

	if ( me->query_skill("jiuyin-zhengong", 1) < 160)
	return notify_fail("你的九阴真功火候不够，无法施展出玄阴诀。\n");

	if ( me->query_skill("jiuyin-shenfa", 1) < 160)
	return notify_fail("你的九阴身法火候不够，无法施展出玄阴诀。\n");

	if ( me->query("max_neili") < 2000)
	return notify_fail("你的内力不够，无法施展出玄阴诀。\n");

	if ( me->query("neili") < 1000)
	return notify_fail("你的真气不够，无法施展出玄阴诀。\n");

	if ( me->query("jingli") < 1000)
	return notify_fail("你的精气不够，无法施展出玄阴诀。\n");

	message_vision(HIB"$N使出九阴「玄阴诀」，迅捷无比的攻向$n！\n"NOR,me, target);

	me->add("neili", -500);
	me->add("jingli", -150);
	
	me->add_temp("apply/pfm_attack", 480+lvl/10);
	me->add_temp("apply/pfm_damage", 250+lvl/4);
	me->add_temp("apply/damage", 350+lvl);

	message_vision(HIB"$N连出数剑，一剑比一剑凌利！\n"NOR,me,target);
	message_vision(HIB"$n连连後退，防守尚有困难，更别说攻击了。\n"NOR,me,target);

	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);

	if ( me->is_fighting(target))
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);

	if ( me->is_fighting(target))
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);

	me->add_temp("apply/pfm_attack", -480-lvl/10);
	me->add_temp("apply/pfm_damage", -250-lvl/4);
	me->add_temp("apply/damage", -350-lvl);


	if(COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 0,1)) target->start_busy(3+random(5));
	return 1;
}

string perform_name(){ return HIB"玄阴诀"NOR; }

int help(object me)
{
        write(HIB"\n玄阴剑法之「玄阴诀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 2000 以上；
                当前精力 1000 以上；
                玄阴剑法等级 160 以上；
                九阴真功等级 160 以上；
                九阴身法等级 160 以上；
                激发剑法为玄阴剑法；
                激发招架为玄阴剑法；
                且手持兵器。

HELP
        );
        return 1;
}
