// 掌刀
// Modified by snowman@SJ 17/12/2000
#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 3;}

string perform_name(){ return HBBLU"铁掌掌刀"NOR; }
int perform(object me, object target)
{
	int lvl;
	object weapon;

	if( !target ) target = offensive_target(me);

	if( !target
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target)
	|| environment(target)!=environment(me)
	|| target == me )
	  return notify_fail("掌刀只能对战斗中的对手使用。\n");

	if((int)me->query("neili") < 1500  )
	  return notify_fail("你的内力不够。\n");

	if( me->query_temp("weapon") )
	  return notify_fail("你必须空手使用「掌刀」。\n");

	if((int)me->query_temp("tzzf") )
	  return notify_fail("你已经在掌刀的运功中。\n");

	if((int)me->query_skill("tiezhang-zhangfa", 1) < 300 )
	  return notify_fail("你的铁掌掌法还不够娴熟，使不出「掌刀」来。\n");

	if (me->query_skill_mapped("force") != "guiyuan-tunafa")
	  return notify_fail("你所使用的内功不对。\n");

	if (me->query_skill_prepared("strike") != "tiezhang-zhangfa"
	|| me->query_skill_mapped("strike") != "tiezhang-zhangfa"
	|| me->query_skill_mapped("parry") != "tiezhang-zhangfa")
	  return notify_fail("你现在无法使用「掌刀」！\n");

	lvl = me->query_skill("tiezhang-zhangfa",1);


	weapon = new(BINGQI_D("tz_noshow"));
	if( !weapon )
	  return notify_fail("Error! 掌刀出错，请通知巫师： cannot load tiezhang ！\n");
	weapon->move(me);
	weapon->set("sharpness", me->query("str")/3);
	me->set_temp("weapon", weapon);
	me->add("neili", -500-lvl);
	me->add_temp("apply/damage", 500+lvl/2);
	me->add_temp("apply/pfm_attack", 150+lvl/10);
	me->add_temp("apply/pfm_damage", 200+lvl/5);
	message_vision(WHT"\n$N突然面色苍白，双手颤抖，随即两眼神光一闪，一双手掌颜色变得"HIW"雪白"WHT"，发出一种奇怪的光芒！\n" NOR, me, target);
	me->set_temp("tzzf/wp", weapon);
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);

	call_out("remove_effect", 1 , me, lvl, weapon,lvl/20);
	me->start_perform(2, "掌刀");
	me->start_busy(random(2));
	return 1;
}

void remove_effect(object me, int lvl, object weapon, int i)
{
	object wp;

	if ( !me ) {
		return;
	}
	wp = me->query_temp("tzzf/wp");

	if( me->query_temp("weapon") != wp
	|| me->query_skill_mapped("strike") != "tiezhang-zhangfa"
	|| i < 1
	|| !me->is_fighting()
	|| me->query("neili") < 1500 ){
		destruct(wp);
  	me->add_temp("apply/damage", -500-lvl/2);
	  me->add_temp("apply/pfm_attack", -150-lvl/10);
	  me->add_temp("apply/pfm_damage", -200-lvl/5);
		me->delete_temp("tzzf");
		me->delete_temp("weapon");
		tell_object(me, WHT"\n你掌刀运功完毕，双手逐渐恢复了原状。\n" NOR);
		return;
	}
	me->add("neili", -100);
	call_out("remove_effect", 1 , me, lvl, weapon,i--);
}

int help(object me)
{
        write(WHT"\n铁掌掌法「铁掌掌刀」："NOR"\n");
        write(@HELP
        这是裘千仞铁掌功的绝招，双掌刀剑不伤，坚硬无比，掌出如刀，宛如一
        绝世兵器！
        
        要求：  内力 1500 以上；       
                铁掌掌法等级 300 以上；
                且手无兵器。
HELP
        );
        return 1;
}
