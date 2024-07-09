// yuxiao.c 碧海潮生按玉箫

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 2;}

int perform(object me, object target)
{
	object weapon;
	int lvl;

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !target->is_character() || !me->is_fighting(target) )
		return notify_fail("碧海潮生按玉箫只能对战斗中的对手使用。\n");

	if (!(me->query("thd/perform") & 1))
		return notify_fail("你虽然听说过“碧海潮生按玉箫”这一招，可是却未获传授。\n");

	if( (lvl=(int)me->query_skill("yuxiao-jian", 1) )< 100 )
		return notify_fail("你的玉箫剑法不够娴熟，无法使用碧海潮生按玉箫。\n");

	weapon = me->query_temp("weapon");
	if (!weapon || !(weapon->id("xiao")))
		return notify_fail("你手里的兵器不是箫，无法使用碧海潮生按玉箫。\n");

	if (me->query_skill_mapped("sword") != "yuxiao-jian")
		return notify_fail("你暂时无法使用碧海潮生按玉箫。\n");

	if ( (int)me->query("neili") < 500)
		return notify_fail("你的内力不够，无法使用碧海潮生按玉箫。\n");

	me->set_temp("apply/action/pfm", CYN"$N运起内功，将箫凑到嘴边，吹得两下，只见箫筒里突然射出一道青光，直扑$n");
	target->set_temp("apply/action/dodge", "可惜$n早觉有异，向旁边一跃，躲开了这个暗器，但对暗器来势如此迅猛也不自禁地露出一丝惧意。\n" NOR);
	target->set_temp("apply/action/parry", "可惜$n早觉有异，向旁边一跃，躲开了这个暗器，但对暗器来势如此迅猛也不自禁地露出一丝惧意。\n" NOR);

	me->receive_damage("neili", 500+lvl/2);
	me->add_temp("apply/damage",1500);
	me->add_temp("apply/pfm_damage",300);
	me->add_temp("apply/pfm_attack",360);
	COMBAT_D->do_attack(me, target,weapon,random(2)?1:3);
	me->add_temp("apply/damage",-1500);
	me->add_temp("apply/pfm_damage",-300);
	me->add_temp("apply/pfm_attack",-360);
	me->delete_temp("apply/action/");

	me->start_busy(random(2));

	return 1;
}

string perform_name(){ return CYN"碧海潮生按玉箫"NOR; }

int help(object me)
{
        write(CYN"\n「碧海潮生按玉箫」："NOR"\n");
        write(@HELP


        要求：  碧海潮生等级 100 以上；
                玉箫剑法等级 100 以上；              
                当前内力  500以上；
                手中持箫。

HELP
        );
        return 1;
}
