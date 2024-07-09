// By Darken@SJ
#include <ansi.h>
inherit F_SSERVER;


string perform_name() { return HIG"神通剑气"NOR; }

int perform(object me, object target)
{
	int lvl;
	object weapon;
	string msg;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)  || !target->is_character() || !me->is_fighting(target)
	|| environment(target)!=environment(me))
	return notify_fail("弹指神通只能对战斗中的对手使用。\n");
	if( (lvl = (int)me->query_skill("tanzhi-shentong", 1)) < 150 )
	return notify_fail("你的弹指神通还未到火候，使不出来神通剑气。\n");
	if( (int)me->query_skill("bihai-chaosheng", 1) < 150)
	return notify_fail("你的碧海潮生功还未到火候。\n");
	if( (int)me->query_skill("throwing", 1) < 150 )
	return notify_fail("你的基本暗器还未到火候，使不出来神通剑气。\n");
	if( (int)me->query("max_neili", 1) < 2000 )
	return notify_fail("你的内功修为不足以使出神通剑气。\n");
	if( (int)me->query("neili", 1) < 1000 )
	return notify_fail("你的内力不足以使出神通剑气。\n");
	if (!objectp(weapon = me->query_temp("weapon"))
	&& (int)me->query_skill("tanzhi-shentong", 1) < 160)
	return notify_fail("你现在无法使用神通剑气。\n");
	if(weapon && (weapon->query("skill_type") != "throwing"
	|| me->query_skill_mapped("throwing") != "tanzhi-shentong"))
	return notify_fail("你现在的武器无法使用神通剑气。\n");
	if(weapon){
		if(weapon->query("skill_type") != "throwing" || me->query_skill_mapped("throwing") != "tanzhi-shentong")
		return notify_fail("你现在的武器无法使用神通剑气。\n");
		msg = HIW"只见$N"HIW"双肩微晃，凝力于指，将"+weapon->name()+HIW+"化作一道闪光从$P手中射出，直奔$n"HIW"而去";
	}
	else msg = HIG"$N暗运内劲于指，看准机会对着$n一弹，一丝气劲激射而出，点向$p胸前大穴";

	me->add("neili", -350-lvl/2);
	me->set_temp("apply/action/pfm",msg);
  
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_hurt",350+lvl/2);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	COMBAT_D->do_attack(me, target,0,random(2)?0:3,2);
	me->add_temp("apply/pfm_hurt",-350-lvl/2);
	me->add_temp("apply/pfm_attack",-180-lvl/10);	   
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
        write(HIG"\n「神通剑气」："NOR"\n");
        write(@HELP

        施展弹指神通的特殊诀窍，将碧海潮生功的内劲凝聚如同一把细剑攻击对手
        或是将暗器附加剑气射出。

        要求：  碧海潮生等级 150 以上；
                弹指神通等级 150 以上；
                基本指法等级 150 以上；
                基本暗器等级 150 以上；
                最大内力 2000 以上；
                当前内力 1000 以上；
                可配合暗器使用。
HELP
        );
        return 1;
}

