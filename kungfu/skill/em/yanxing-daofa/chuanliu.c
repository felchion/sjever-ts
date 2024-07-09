// chuanliu.c
#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

int query_ap() {return 5;}

int perform(object me, object target)
{
	object weapon;
	int lvl;
	weapon = me->query_temp("weapon");

	if( !target ) target = offensive_target(me);
	
	if( !objectp(target)
	 || !me->is_fighting(target) )
		return notify_fail("你只能在战斗中使用雁行穿柳刀。\n");

	if (!objectp(weapon) || (string)weapon->query("skill_type") != "blade")
		return notify_fail("你使用的武器不对。\n");

	if((int)me->query_skill("linji-zhuang", 1) < 180 )
		return notify_fail("你的临济十二庄等级不够，不能使用雁行穿柳刀。\n");

	if (me->query_skill_mapped("force") != "linji-zhuang")
		return notify_fail("你所使用的内功不对。\n");

	if ((int)me->query_skill("yanxing-daofa", 1) < 180 )
		return notify_fail("你的雁行刀法等级不够，不能使用雁行穿柳刀！\n");

	if ((int)me->query_skill("blade", 1) < 180 )
		return notify_fail("你的基本刀法极不够娴熟，不能使用雁行穿柳刀！\n");

	if ( me->query_skill_mapped("blade") != "yanxing-daofa"
	 || me->query_skill_mapped("parry") != "yanxing-daofa" )
		return notify_fail("你现在无法使用雁行穿柳刀！\n");

	if ((int)me->query("neili") < 1000 )
		return notify_fail("你现在真气太弱，不能使用雁行穿柳刀！\n");

	if ((int)me->query("max_neili") < 2000 )
		return notify_fail("你现在内力太弱，不能使用雁行穿柳刀！\n");

	if (me->query_temp("yanxing/pfm") ) 
		return notify_fail("你已经在使用雁行穿柳刀了！\n");

	message_vision(HIY"\n$N运起临济十二庄，手中" + weapon->name() +HIY"陡然一亮。\n"NOR, me);
	lvl = me->query_skill("yanxing-daofa",1);

  me->add_temp("apply/pfm_damage", 180+lvl/5);
  me->add_temp("apply/pfm_attack", 240+lvl/10);
  me->add_temp("apply/damage", 500+lvl/2);
  me->set_temp("yanxing/pfm",1);

	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
	if ( me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
 
	me->add("neili", -300);
	me->start_busy(random(2));
	call_out("remove_effect", 1, me, target, weapon, lvl);
	return 1;
}

void remove_effect(object me, object target,object weapon, int lvl)
{
	if( !me) return;
	if(!me->is_fighting(target)
	|| ! weapon
	|| me->query_temp("weapon") != weapon
	|| me->query_skill_mapped("blade") != "yanxing-daofa"
	|| me->query_skill_mapped("parry") != "yanxing-daofa"){
		if( weapon)
		message_vision(HIW"$N手上的刀光渐渐的消失了！\n"NOR, me);
	  me->add_temp("apply/pfm_damage", -180-lvl/5);
	  me->add_temp("apply/pfm_attack", -240-lvl/10);
	  me->add_temp("apply/damage", -500-lvl/2);
	  me->delete_temp("yanxing/pfm");
		return;
	}
	call_out("remove_effect", 1, me, target, weapon, lvl);
}

string perform_name(){ return HIY"雁行穿柳刀"NOR; }

int help(object me)
{
        write(HIY"\n雁行刀法之「雁行穿柳刀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 2000 以上；
                雁行刀法等级 180 以上；
                临济十二庄等级 180 以上；
                基本刀法等级 180 以上；
                激发刀法为雁行刀法；
                激发招架为雁行刀法；
                激发内功为临济十二庄；
                且手持兵器。

HELP
        );
        return 1;
}
