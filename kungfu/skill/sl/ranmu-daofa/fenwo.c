// renmu-daofa 燃木刀法
// by snowman@SJ 05/12/1999

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 3;}
string perform_name() {return HIR"焚我真火"NOR;}


int perform(object me, object target)
{
	object weapon;
	int i,lvl;
	weapon = me->query_temp("weapon");

	if( !target ) target = offensive_target(me);

	if( !target
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("「焚我真火」攻击只能对战斗中的对手使用。\n");

	if (!weapon || weapon->query("skill_type") != "blade"
	|| me->query_skill_mapped("blade") != "ranmu-daofa")
	return notify_fail("你现在无法使出「焚我真火」绝技！\n");

	if (weapon->query("sharpness") > 3)
	return notify_fail("你手中之刀已是锋利之极，使用「焚我真火」绝技没有什么作用！\n");

	if (weapon->query("unique"))
	return notify_fail("你手中之刀已是锋利之极，使用「焚我真火」绝技没有什么作用！\n");

	if((lvl = me->query_skill("ranmu-daofa", 1))< 150 )
	return notify_fail("你的燃木刀法火候不够，使不出「焚我真火」绝技。\n");

	if(me->query_skill("blade", 1) < 150 )
	return notify_fail("你的基本功还不够娴熟，使不出「焚我真火」绝技。\n");

	if( (int)me->query_skill("yijin-jing", 1) < 150 )
	return notify_fail("你的易筋经等级不够，使不出「焚我真火」绝技。\n");

	if( (int)me->query_str() < 35)
	return notify_fail("你的膂力还不够强劲，使不出「焚我真火」绝技。\n");

	if( (int)me->query_int() < 30)
	return notify_fail("没有大智慧大般若，是使不出「焚我真火」绝技的。\n");

	if ( me->query_skill_mapped("force") != "yijin-jing")
	return notify_fail("没有易筋经作为根基，是无法使出「焚我真火」绝技的！\n");

	if (me->query("max_neili") < 2000)
	return notify_fail("你的内力修为不够。\n");

	if (me->query("neili") < 1500)
	return notify_fail("你的内力不够。\n");

	i = me->query("fenwo");
	if (i > lvl*10) i = lvl*10;
	me->set_temp("fenwo_hit", 1);

	me->set_temp("apply/action/pfm", HIR"$N双掌夹刀合十，手中的"+weapon->name()+HIR"顿时升起一团火焰将自己包围，$P猛宣一声佛号，一刀直劈向$n");

	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/damage",400+i/5);
	me->add_temp("apply/pfm_damage",200+i/20);
	me->add_temp("apply/pfm_attack",180+i/100);
	COMBAT_D->do_attack(me, target,weapon,random(i)>lvl*5?3:1);
	me->add_temp("apply/damage",-400-i/5);
	me->add_temp("apply/pfm_damage",-200-i/20);
	me->add_temp("apply/pfm_attack",-180-i/100);
	me->delete_temp("apply/action/");

	weapon->add("fenwo", 1);
	if (weapon
	&& !weapon->query("unique")
	&& weapon->query("rigidity") < 2
	&& weapon->query("fenwo") >= (3+weapon->query("rigidity"))) {
		message_vision(HIY"\n只听“咔咔”几声，$N手中的$n"HIY"在熊熊火焰中裂成了碎片！\n"NOR,me,weapon);
		weapon->unequip();
		weapon->move(environment(me));
		weapon->set("name", "碎裂的" + weapon->query("name"));
		weapon->set("value", 49);
		weapon->set("weapon_prop", 0);
		me->reset_action();
	}
	me->start_busy(random(2));
	me->add("fenwo", 1);
	return 1;
}

int help(object me)
{
        write(HIR"\n燃木刀法之「焚我真火」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 2000 以上；
                后天臂力 35 以上；
                后天悟性 30 以上；
                燃木刀法等级 150 以上；
                易筋经等级 150 以上；
                基本刀法等级 150 以上；
                激发刀法为燃木刀法；

HELP
        );
        write("注：你现已经使用「焚我真火」"+HIY+chinese_number(me->query("fenwo"))+NOR+"次。\n");
        return 1;
}   