// yunu-jianfa 玉女素心
// by dubei
// Modify By River 99.5.26
#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

string perform_name(){ return HIM"玉女素心"NOR; }
int query_ap() {return 3;}
int perform(object me, string arg){
	object weapon, target;
	int lvl;

	lvl = me->query_skill("yunu-xinjing",1);
	if (me->query("gender") == "男性") lvl = lvl/2;
	
	weapon = me->query_temp("weapon");

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) )
		return notify_fail("「玉女素心」只能在战斗中对对手使用。\n");

	if ( !weapon
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "yunu-jianfa")
		return notify_fail("你没用玉女素心剑怎么使出「玉女素心」呢？\n");

	if((int)me->query_skill("yunu-jianfa", 1) < 100 )
		return notify_fail("你的玉女剑法修为不够 。\n");

	if((int)me->query_skill("sword", 1) < 100 )
		return notify_fail("你的基本功还不够娴熟，使不出「玉女素心」绝技。\n");

	if((int)me->query_skill("yunu-xinjing", 1) < 100 )
		return notify_fail("你的玉女心经修为不够，无法使用「玉女素心」绝技。\n");
	
	if( me->query_skill_mapped("force") != "yunu-xinjing" && userp(me))
		return notify_fail("你现在无法使用「玉女素心」！\n");
	
	if((int)me->query_temp("gumu_suxin"))
		return notify_fail("你现在正在使用「玉女素心」绝技。\n");
	
	if((int)me->query_temp("gm/sanqing"))
		return notify_fail("你现在正在使用「一气化三清」绝技。\n");

	if( me->query_temp("wujian") )
		return notify_fail("你正在使用「无剑胜有剑」呢！\n");

	if((int)me->query_temp("hebi"))
		return notify_fail("你正在施展双剑合壁，无法使用「玉女素心」绝技。\n");

	if((int)me->query("max_neili") < 1500)
		return notify_fail("你的内力修为不够。\n");

	if((int)me->query("neili") < 700)
		return notify_fail("你的内力不够。\n");

	me->add("neili", -200-lvl/2);
	me->add("jingli", -100);
	me->add_temp("apply/damage", 230+lvl);
	me->add_temp("apply/pfm_damage", 180+lvl/4);
	me->add_temp("apply/pfm_attack", 180+lvl/10);
	me->add_temp("apply/pfm_dodge", 150+lvl/5);
	me->add_temp("apply/pfm_parry", 100+lvl/10);
	if (lvl > 399)	{
		me->add_temp("apply/rush", 20 + lvl/40);
		me->set_temp("apply/action/color",HIW);
	}
	me->set_temp("gumu_suxin", 1);
	message_vision(HIW"\n$N信手一剑，极尽潇洒脱逸之妙，手中"+weapon->name()+HIW"幻化出无数虹芒，向$n电掣攻去。\n"NOR,me,target);
	target->set_temp("must_be_hit",1);
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);
	if (objectp(target)) 
		target->delete_temp("must_be_hit");
	me->start_perform(3,"「玉女素心」");
	call_out("check_fight", 1, me, lvl, 8);
	return 1;
}

void remove_effect(object me, int lvl, object weapon){
	if (!me) return;
	me->add_temp("apply/damage", -230-lvl);
	me->add_temp("apply/pfm_damage", -180-lvl/4);
	me->add_temp("apply/pfm_attack", -180-lvl/10);
	me->add_temp("apply/pfm_dodge", -150-lvl/5);
	me->add_temp("apply/pfm_parry", -100-lvl/10);
	if (lvl > 399)	{
		me->add_temp("apply/rush", -20 - lvl/40);
		me->delete_temp("apply/action/color");
	}
	me->delete_temp("gumu_suxin");
	me->start_busy(random(2)+1);
	tell_object(me, HIW"你「玉女素心」施展完毕，缓缓舒了一口气。\n" NOR);
}

void check_fight(object me, int lvl, int count){
	object weapon;
	if (!me) return;
	if(!objectp(weapon = me->query_temp("weapon"))
	|| me->query_skill_mapped("sword") != "yunu-jianfa"
	|| me->query_skill_mapped("parry") != "yunu-jianfa"
	|| me->query_skill_mapped("force") != "yunu-xinjing"
	|| weapon->query("skill_type") != "sword"){
		remove_effect(me, lvl, weapon);
		return;
	}
	if( count < 1 || !me->is_fighting()) {
		remove_effect(me, lvl, weapon);
		return;
	}
	call_out("check_fight", 1, me, lvl, count-1);
}

int help(object me)
{
        write(HIM"\n玉女剑法之「玉女素心」："NOR"\n\n");
        write(@HELP
        要求：  当前内力  700 以上；
                最大内力 1500 以上；
                玉女剑法等级 100 以上；
                玉女心经等级 100 以上；
                激发剑法为玉女剑法；

HELP
        );
        return 1;
}   