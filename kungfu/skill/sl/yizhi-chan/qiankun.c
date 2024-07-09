// qiankun.c
// by snowman@SJ 22/06/99

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 3;}

int perform(object me, object target)
{
	int lvl;

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) || !living(target)
	|| environment(target)!=environment(me))
	return notify_fail("「一指乾坤」只能在战斗中对对手使用。\n");

	if( me->query_temp("weapon") )
	return notify_fail("你必须空手使用「一指乾坤」！\n");

	if( (lvl =(int)me->query_skill("yizhi-chan",1)) < 180 )
	return notify_fail("你的一指禅不够娴熟，不会使用「一指乾坤」！\n");

	if (me->query_skill_prepared("finger") != "yizhi-chan"
	|| me->query_skill_mapped("finger") != "yizhi-chan")
	return notify_fail("你现在无法使用「一指乾坤」进行攻击。\n");

	if( (int)me->query_skill("yijin-jing",1) < 180 )
	return notify_fail("「一指乾坤」需要雄厚的易筋经内功为基础。\n");

	if (me->query_skill_mapped("force") != "yijin-jing")
	return notify_fail("你所使用的内功不对。\n");

	if( (int)me->query("max_neili") < 3000 )
	return notify_fail("你的内力修为太次，不能使用「一指乾坤」！\n");

	if( (int)me->query("neili") < 1000 )
	return notify_fail("你的内力太少了，无法使用出「一指乾坤」！\n");

	if (me->query_skill_prepared("finger") != "yizhi-chan"
	|| me->query_skill_mapped("finger") != "yizhi-chan")
	return notify_fail("你现在无法使用「一指乾坤」进行攻击。\n");

	me->set_temp("apply/action/pfm", HIC"\n$N面露微笑，曲指对着$n轻轻一弹，只听嗤的一声一股劲气从$P手指激射而出，窜向$p丹田");
	me->set_temp("apply/action/hit", HIW"结果$n一个不小心，被指劲点在丹田之上，顿时丹气混乱，脸色发白！\n"NOR);

	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/force",300+lvl);
	me->add_temp("apply/pfm_force",250+lvl/4);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	if (COMBAT_D->do_attack(me, target,0,random(2)?0:3) ) target->apply_condition("yzc_qiankun", 4+random(4));
	me->add_temp("apply/force",-300-lvl);
	me->add_temp("apply/pfm_force",-250-lvl/4);
	me->add_temp("apply/pfm_attack",-180-lvl/10);
	me->delete_temp("apply/action/");
	me->start_busy(random(2));
	return 1;
}



string perform_name(){ return HIC"一指乾坤"NOR; }

int help(object me)
{
        write(
@HELP
       
要求：
		最大内力 3000 以上；      
		当前内力 1000 以上；  
		易筋经等级 180 以上；
		一指禅等级 180 以上；
		激发指法为一指禅。
  
HELP
        );
        return 1;
}