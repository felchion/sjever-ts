// qixing. 七星绝命剑
// Modify By River 99.5.26
// Modified by darken@SJ
#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 2;}
string perform_name() {return HIW"七星绝命剑"NOR;}

int perform(object me, object target)
{
	int lvl;
	object weapon = me->query_temp("weapon");
	
	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("你只能对战斗中的对手使用「七星绝命剑」绝技。。\n");

	if( (lvl=(int)me->query_skill("quanzhen-jianfa", 1)) < 120 )
	return notify_fail("你的全真剑法不够熟练，使不出「七星绝命剑」绝技。\n");

	if ( !weapon
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "quanzhen-jianfa"
	|| me->query_skill_mapped("parry") != "quanzhen-jianfa")
	return notify_fail("你现在无法使用七星绝命剑。\n");

	if((int)me->query_skill("sword", 1) < 120 )
	return notify_fail("你的基本功还不够娴熟，使不出「七星绝命剑」绝技。\n");

	if((int)me->query_dex() < 25)
	return notify_fail("你的身法还不够迅，使不出「七星绝命剑」绝技。\n");

	if((int)me->query_skill("force", 1) < 120)
	return notify_fail("你的内功修为不够，无法使用「七星绝命剑」绝技。\n");

	if((int)me->query("neili") < 1000)
	return notify_fail("你的内力不够，无法使用「七星绝命剑」绝技。\n");

	me->set_temp("apply/action/pfm", HIW"\n$N一声长啸，一抖手中剑，剑幕中飞出七朵寒星，既如狂风暴雨又如流星般向$n罩了过去");
	me->set_temp("apply/action/hit", HIR"$n躲闪不及，身中七剑，“哇”的狂叫一声，鲜血狂喷而出！\n"NOR);

	me->receive_damage("neili", 500+lvl/2);
	me->add_temp("apply/damage",400+lvl);
	me->add_temp("apply/pfm_damage",200+lvl/4);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	COMBAT_D->do_attack(me, target,weapon,random(2)?1:3);
	me->add_temp("apply/damage",-400-lvl);
	me->add_temp("apply/pfm_damage",-200-lvl/4);
	me->add_temp("apply/pfm_attack",-180-lvl/10);
	me->delete_temp("apply/action/");
  me->start_busy(random(2));
	return 1;
}

int help(object me)
{
        write(HIW"\n全真剑法之「七星绝命剑」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                全真剑法等级 120 以上；
                基本内功等级 120 以上；
                基本剑法等级 120 以上；
                激发剑法为全真剑法；

HELP
        );
        return 1;
}    
