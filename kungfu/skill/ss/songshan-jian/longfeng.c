// longxiang.c 龙吟凤翔
// by xiaoyao 4/19/2k

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

int query_ap() {return 7;}

int perform(object me, object target)
{
	string msg;
	int lvl;
	object weapon;

	lvl = me->query_skill("songshan-jian", 1);
	weapon = me->query_temp("weapon");

	if (!target ) target = offensive_target(me);

	if (!objectp(target)
	|| !me->is_fighting(target) )
	return notify_fail("「龙吟凤翔」只能在战斗中对对手使用。\n");

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "sword")
	return notify_fail("你必须使用剑才能使用「龙吟凤翔」。\n");

	if (me->query_skill("songshan-jian", 1) < 300 )
	return notify_fail("你的嵩山剑法还不够娴熟，使不出「龙吟凤翔」绝技。\n");

	if (me->query_skill("hanbing-zhenqi", 1) < 300 )
	return notify_fail("你的寒冰真气不够高，无法使用「龙吟凤翔」。\n");

	if (me->query_skill("songyang-shou", 1) < 300 )
	return notify_fail("你对大嵩阳手认识还不够！\n");

	if ( me->query_skill("force") < 400 )
	return notify_fail("你的内功等级不够，使不出「龙吟凤翔」绝技。\n");

	if (me->query_skill_prepared("hand") != "songyang-shou"
	|| me->query_skill_mapped("hand") != "songyang-shou")
	return notify_fail("「龙吟凤翔」需要用大嵩阳手辅助。\n");

	if ( me->query_skill_mapped("sword") != "songshan-jian"
	|| me->query_skill_mapped("parry") != "songshan-jian")
	return notify_fail("你现在的状态使不出「龙吟凤翔」绝技。\n");

	if ( me->query_temp("ss_lf"))
	return notify_fail("你正在使用「龙吟凤翔」呢！\n");

	if ( me->query("max_neili") < 2500 )
	return notify_fail("你现在内力太弱，使不出「龙吟凤翔」。\n");

	if ( me->query("neili") < 1500 )
	return notify_fail("你现在真气太弱，使不出「龙吟凤翔」。\n");

	if ( me->query("jingli") < 1000 )
	return notify_fail("你现在精力太弱，使不出「龙吟凤翔」。\n");

	msg = HIR"\n$N伸出左手拇指、食指、中指三根手指，握住了剑尖，右手一松，"+weapon->name()+HIR"突然弹起，剑柄在前，不住地晃动。\n突然$N绕着$n快速攻击，人剑合一，幻作一条金龙腾空而起，自半空中横过，剑身似曲似直，倏的罩向$n！\n\n"NOR;
	message_vision(msg, me, target);
	
	if(me->query_temp("ss/hb"))
		message_vision(HIB"\n$N更将「寒冰诀」发挥得淋漓尽致，身周寒光大盛！\n" NOR,me,target);

	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_attack", 240+lvl/5);
	me->add_temp("apply/damage", 500+lvl);
	me->add_temp("apply/pfm_damage", 250+lvl/4);
	me->set_temp("apply/action/skill","songyang-shou");

	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);

	message_vision(HIR"                                                                   ※※  "HIC"龙"NOR  HIR"  ※※\n"NOR,me);

	weapon->unequip();
	if (me->is_fighting(target)) COMBAT_D->do_attack(me, target, 0, random(2)?1:3);
	message_vision(HIR"                                                                   ※※  "HIC"吟"NOR  HIR"  ※※\n"NOR,me);

	weapon->wield();

	if (me->is_fighting(target)) COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);

	message_vision(HIR"                                                                   ※※  "HIC"凤"NOR  HIR"  ※※\n"NOR,me);

	weapon->unequip();
	if (me->is_fighting(target)) COMBAT_D->do_attack(me, target, 0, random(2)?1:3);
	message_vision(HIR"                                                                   ※※  "HIC"翔"NOR  HIR"  ※※\n"NOR,me);

	weapon->wield();

	if ( lvl > 350 && me->is_fighting(target) ){
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
	}

	me->add_temp("apply/pfm_attack", -240-lvl/5);
	me->add_temp("apply/damage", -500-lvl);
	me->add_temp("apply/pfm_damage", -250-lvl/4);
  me->delete_temp("apply/action");
	me->delete_temp("ss_lf");
	me->start_busy(1);
	return 1;
}

string perform_name(){ return HIR"龙吟凤翔"NOR; }

int help(object me)
{
        write(HIR"\n嵩山剑法之「龙吟凤翔」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1500 以上；
                最大内力 2500 以上；
                当前精力 1000 以上；
                嵩山剑法等级 300 以上；
                寒冰真气等级 300 以上；
                大嵩阳手等级 300 以上；
                有效内功等级 400 以上；
                激发剑法为嵩山剑法；
                激发招架为嵩山剑法；
                激发手法为大嵩阳手；
                备手法为大嵩阳手；
                且手持兵器。

HELP
        );
        return 1;
}
