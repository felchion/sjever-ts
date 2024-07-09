// lianhuan.c for huashan-jianfa
// by tiantian@SJ 11/05/2000.
// update by lsxk@hsbbs 2007/7/26 for增强剑宗使用威力.

#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return MAG"夺命连环三仙剑"NOR;}
int query_ap() {return 3;}

int perform(object me,object target)
{
	int lvl;
	object weapon;

	if( !target ) target = offensive_target(me);

	if(!objectp(target) || !me->is_fighting(target) || !living(target)
	|| environment(target)!=environment(me))
	return notify_fail("夺命连环三仙剑只能对战斗中的对手使用。\n");

	if(me->query_skill_mapped("force") != "huashan-qigong"
	&&me->query_skill_mapped("force") != "zixia-gong" )
	return notify_fail("你的特殊内功不对，无法使用夺命连环三仙剑！\n");
	if( (lvl = (int)me->query_skill("huashan-qigong", 1)) < 120
	&&(int)me->query_skill("zixia-gong", 1) < 120 )
	return notify_fail("你的内功还未练成，不能使用夺命连环三仙剑！\n");
	if( (int)me->query_skill("huashan-jianfa", 1) < 120 )
	return notify_fail("你的剑法还未练成，不能使用夺命连环三仙剑！\n");
	if((int)me->query_skill("sword", 1) < 120 )
	return notify_fail("你的基本剑法不够娴熟，不能在剑招中使用夺命连环三仙剑。\n");
	if (!objectp(weapon = me->query_temp("weapon")) || weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "huashan-jianfa")
	return notify_fail("你手里无剑，如何使用夺命连环三仙剑？\n");

	if((int)me->query("max_neili") < 1500 )
	return notify_fail("你的内力修为不够，不能使用夺命连环三仙剑！\n");
	if((int)me->query("neili") < 1000 )
	return notify_fail("你现在真气不足，不能使用夺命连环三仙剑！\n");
	if((int)me->query_temp("wudui"))
	return notify_fail("你正在使用无双无对！\n");

	message_vision(MAG"\n$N"MAG"长啸一声，使出平生绝技，连环三击，正是华山剑宗绝学「夺命连环三仙剑」"MAG"！\n"NOR, me,target);

	me->set_temp("hsjf/lianhuan",1);
	me->add("neili", -500-lvl/2);

	me->add_temp("apply/damage", 500+lvl/2);
  me->add_temp("apply/pfm_damage", 200+lvl/5);
  me->add_temp("apply/pfm_attack", 150+lvl/10);
  
	COMBAT_D->do_attack(me, target, weapon,random(2)?0:3);
	if (me->is_fighting(target))
		COMBAT_D->do_attack(me, target, weapon,random(2)?0:3);
	if (me->is_fighting(target))
		COMBAT_D->do_attack(me, target, weapon,random(2)?0:3);
		
	me->add_temp("apply/damage", -500-lvl/2);
  me->add_temp("apply/pfm_damage", -200-lvl/5);
  me->add_temp("apply/pfm_attack", -150-lvl/10);

	me->delete_temp("hsjf/lianhuan");
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
   write(WHT"\n华山剑法「"MAG"夺命连环三仙剑"WHT"」："NOR"\n");
   write(@HELP
    这就是华山剑宗绝技「夺命连环三仙剑」！当年华山派剑宗、气宗之
    争，乃在华山玉女峰比剑争夺，剑宗弟子正是用了此招杀害了数名气
    宗好手，回想起当年战场，也令现在的气宗弟子骇然。气宗弟子虽视
    剑宗弟子为魔道，但心下却不禁佩服此招之高明，华山玉女峰一战后
    有少数气宗弟子竟然也在练习此招。

    要求：紫霞神功 120级
          华山剑法120级，
          基本剑法120级，
          最大内力1500以上，
          当前内力1000以上。
HELP
   );
   return 1;
}
