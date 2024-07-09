// tougu.c 透骨针
// by snowman
// Modify By River@SJ 增加有9阳后的效果 2000.2.22
#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 4;}

int perform(object me, object target)
{
	object weapon;
	int lvl;

	weapon = me->query_temp("weapon");

	if( !target) target = offensive_target(me);

	if( !objectp(target)) target = offensive_target(me);
	if( !target || !me->is_fighting(target) || !living(target)
	|| environment(target)!=environment(me))
	return notify_fail("阴风刀只能在战斗中对对手使用。\n");

	if (me->query_skill_mapped("dagger") != "shenghuo-lingfa")
	return notify_fail("你必须先将圣火令法功和圣火神功相互配合。\n");

	if(!weapon)
	return notify_fail("你必须使用武器来传递阴风刀内劲！\n");

	if( weapon->query("skill_type") != "dagger" )
	return notify_fail("你必须使用匕法传递阴风刀内劲！\n");

	if( (int)me->query_skill("shenghuo-shengong",1) < 120 )
	return notify_fail("你的圣火神功等级不够，不能施展阴风刀内劲！\n");

	if( (int)me->query_skill("shenghuo-lingfa",1) < 120 )
	return notify_fail("你的圣火令法等级不够，不能施展阴风刀内劲！\n");

	if( (int)me->query("neili") < 500 )
	return notify_fail("你的真气不足，不能施展阴风刀内劲！\n");

	if( (int)me->query("max_neili") < 1500 )
	return notify_fail("你的内力不足，不能施展阴风刀内劲！\n");

	if( me->query_temp("qkdny/yin") )
	return notify_fail("你正在施展乾坤大挪移「引」字诀呢！\n");

	if( me->query_temp("qkdny/hua") )
	return notify_fail("你正在施展乾坤大挪移「化」字诀呢！\n");


	me->set_temp("apply/action/pfm", HIW"$n悄悄使出"BLU"阴风刀"HIW"，运起一股无形无质的阴寒之气，直插$n胸口玉堂要穴");
	me->set_temp("apply/action/hit", HIB"一股阴劲如刀、如剑、如匕、如凿，直插入$n胸口的「玉堂穴」中！霎时之间，$n全身冰冷，几首闭气窒息！\n"NOR);
	target->set_temp("apply/action/parry", HIY"结果阴风刀内劲还没触到$n便已经散了。\n"NOR);
	target->set_temp("apply/action/dodge", HIY"结果阴风刀内劲还没触到$n便已经散了。\n"NOR);

	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_hurt",500+lvl/2);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	COMBAT_D->do_attack(me, target,0,random(2)?0:3,2);
	me->add_temp("apply/pfm_hurt",-500-lvl/2);
	me->add_temp("apply/pfm_attack",-180-lvl/10);
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/");
	me->start_busy(random(2));
	return 1;
}

string perform_name(){ return HIW"阴风刀"NOR; }

int help(object me)
{
        write(HIW"\n圣火令法之「阴风刀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 1500 以上；
                圣火神功等级 120 以上；
                圣火令法等级 100 以上。

HELP
        );
        return 1;
}
