// By Darken@SJ

#include <ansi.h>
inherit F_SSERVER;


int query_ap() {return 4;}

string perform_name() { return HIG"玉漏催银箭"NOR; }

int perform(object me, object target)
{
	object weapon, *arm;
	int flag,i, lvl = (me->query_skill("sword") + me->query_skill("throwing"))/3;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("你只能对战斗中的对手使用玉漏催银剑。\n");

	if( !objectp(weapon = me->query_temp("weapon"))
	|| (weapon->query("skill_type") != "sword" && weapon->query("skill_type") != "xiao"))
	return notify_fail("你手中无剑，无法施展玉漏催银剑！\n");

	if( (int)me->query_skill("yuxiao-jian", 1) < 250 )
	return notify_fail("你的玉箫剑法不够娴熟，无法施展出玉漏催银剑。\n");
	if ((int)me->query_skill("bihai-chaosheng", 1) < 250)
	return notify_fail("你的碧海潮生功火候不够，无法施展出玉漏催银剑。\n");
	if ((int)me->query_skill("tanzhi-shentong", 1) < 250)
	return notify_fail("你的弹指神通火候不够，无法施展出玉漏催银剑。\n");
	if ((int)me->query_skill("throwing", 1) < 250)
	return notify_fail("你的暗器技巧不够，无法施展出玉漏催银剑。\n");
	if (me->query_skill_prepared("finger") != "tanzhi-shentong")
	return notify_fail("你必须以弹指神通做为空手的准备武技。\n");
	if ( (int)me->query("max_neili") < 4500)
	return notify_fail("你的内力不够，无法施展出玉漏催银剑。\n");
	if ( (int)me->query("neili") < 2000)
	return notify_fail("你的真气不够，无法施展出玉漏催银剑。\n");
	if ((int)me->query("jingli") < 2000)
	return notify_fail("你的精气不够，无法施展出玉漏催银剑。\n");

	me->add("neili", -(lvl +random(200)) );
	me->add("jingli", -250);

	me->set_temp("apply/action/pfm" ,HBGRN+HIW+"$N运起弹指神通中指发劲，正是一式玉漏催银箭！只见"+weapon->query("name")+HBGRN+HIW"脱手飞出，风驰电掣般射向$n");

	me->add_temp("apply/pfm_attack",240+lvl/10);
	me->add_temp("apply/damage",350+lvl);
	me->add_temp("apply/pfm_damage",250+lvl/4);
	flag=COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);
	me->add_temp("apply/damage",-350-lvl);
	me->add_temp("apply/pfm_damage",-250-lvl/4);
	me->add_temp("apply/pfm_attack",-240-lvl/10);
	
	weapon->move(environment(me));
	lvl += weapon->query("sharpness")*100;

	if (flag) {
		weapon = target->query_temp("weapon");
		arm = all_inventory(target);
		for (i=0;i<sizeof(arm);i++) {
			if (arm[i]->query("equipped")
			&& random(arm[i]->query("armor_prop/armor")) < lvl /10
			&& random(arm[i]->query("rigidity")) < lvl /100) {
				message_vision(HIR"$N的"+arm[i]->query("name")+HIR"被这凌厉一击击毁了！\n"NOR, target);
				destruct(arm[i]);
			}
		}
		if (weapon && random(weapon->query("rigidity")) < lvl /100) {
			message_vision(HIR"$N的"+weapon->query("name")+HIR"被这凌厉一击击毁了！\n"NOR, target);
			destruct(weapon);
		}
		target->reset_action();
	}

	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
        write(HIG"\n「玉漏催银箭」："NOR"\n");
        write(@HELP

        配合弹指神通绝技的剑法，以手中之箫做为暗器，挟带碧海潮生功的浑厚内
        力，击毁对手身上的防具以及武器。

        要求：  碧海潮生等级 250 以上；
                弹指神通等级 250 以上；
                基本暗器等级 250 以上；
                玉箫剑法等级 250 以上；
                最大内力 4500 以上；
HELP
        );
        return 1;
}

