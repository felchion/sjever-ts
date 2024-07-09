// feiying.c
// YUJ@SJ 2002-07-12

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 8;}

int perform(object me, object target)
{
	object weapon;
	int lvl, i;

	lvl = me->query_skill("yuxiao-jian", 1);
	lvl += me->query_skill("tanzhi-shentong", 1);
	lvl += me->query_skill("luoying-zhang", 1);
	lvl += me->query_skill("lanhua-shou", 1);
	lvl += me->query_skill("xuanfeng-tui", 1);
	lvl += me->query_skill("suibo-zhuliu", 1);
	lvl /= 6;

	if (!target) target = offensive_target(me);

	if (!objectp(target)
	|| !me->is_fighting(target))
		return notify_fail("你只能对战斗中的对手使用飞影。\n");

	weapon = me->query_temp("weapon");

	if (!weapon || (!(weapon->id("xiao")) && weapon->query("skill_type")!="xiao"))
		return notify_fail("你手里没有拿箫，无法施展出飞影。\n");

	if (me->query_skill("yuxiao-jian", 1) < 350)
		return notify_fail("你的玉箫剑法不够娴熟，无法施展出飞影。\n");

	if (me->query_skill("qimen-bagua", 1) < 180)
		return notify_fail("你的奇门八卦不够娴熟，无法施展出飞影。\n");

	if (me->query_skill("tanzhi-shentong", 1) < 350)
		return notify_fail("你的弹指神通不够娴熟，无法施展出飞影。\n");

	if (me->query_skill("lanhua-shou", 1) < 350)
		return notify_fail("你的兰花拂穴手不够娴熟，无法施展出飞影。\n");

	if (me->query_skill("luoying-zhang", 1) < 350)
		return notify_fail("你的落英神剑掌不够娴熟，无法施展出飞影。\n");

	if (me->query_skill("xuanfeng-tui", 1) < 350)
		return notify_fail("你的旋风扫叶腿不够娴熟，无法施展出飞影。\n");

	if (me->query_skill("bihai-chaosheng", 1) < 350)
		return notify_fail("你的碧海潮生功火候不够，无法施展出飞影。\n");

	if (me->query_skill("suibo-zhuliu", 1) < 350)
		return notify_fail("你的随波逐流火候不够，无法施展出飞影。\n");

	if ( me->query_skill_mapped("sword") != "yuxiao-jian"
	|| me->query_skill_mapped("parry") != "yuxiao-jian"
	|| me->query_temp("thd/feiying"))
		return notify_fail("你暂时无法使用飞影绝技。\n");

	if (me->query("max_neili") < 7500)
		return notify_fail("你的内力不够，无法施展出飞影。\n");

	if (me->query("neili") < 2000)
		return notify_fail("你的真气不够，无法施展出飞影。\n");

	if (me->query("jingli") < 2500)
		return notify_fail("你的精气不够，无法施展出飞影。\n");

	me->set_temp("thd/feiying", 1);
	me->add("neili", -450-lvl/2);
	me->add("jingli", -180);

	message_vision(HIW"\n$N深吸一口气，使出桃花绝技「飞影」攻向$n！\n\n"NOR,me, target);

	i = me->query_temp("apply/wp_damage")/2;
	
	me->add_temp("apply/pfm_attack", 240+lvl/10);
	me->add_temp("apply/damage", 350+lvl);
	me->add_temp("apply/pfm_damage", 180+lvl/4+i);

	message_vision(HIG"$N手中的"+weapon->query("name")+HIG"如行云流水一般，向$n潇洒地攻去！"NOR,me, target);
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
	message_vision(HIG"$n只觉得四面八方都是箫影，不知所措！\n"NOR,me, target);
	weapon->unequip();
	if (target->query("qi") > 0 && me->is_fighting(target)) {
		message_vision(HIC"\n$N把"+weapon->query("name")+HIC"插回腰间，双手如清风拂晓般向$n拂去！"NOR,me, target);
		me->set_temp("apply/action/skill","lanhua-shou");
		if (COMBAT_D->do_attack(me, target, 0,1))
		message_vision(HIC"$N正好拂到了$n的小腹关元穴！$n觉得真气无法重新凝聚！\n"NOR,me, target);
	}
	if ( target->query("qi") > 0 && me->is_fighting(target)) {
		message_vision(HIM"\n$N倏地转手为掌，$n觉得到处是掌影，虚虚实实无法捉摸。"NOR,me, target);
		me->set_temp("apply/action/skill","luoying-zhang");
		if (COMBAT_D->do_attack(me, target, 0, 1))
		message_vision(HIM"$N突然一掌打在$n后心至阳穴上，$n不由得吐了口鲜血。\n"NOR,me, target);
	}
	if ( target->query("qi") > 0 && me->is_fighting(target)) {
		message_vision(HIR"\n$N尚未收掌，旋风扫叶腿已经攻到$n面前！"NOR, me, target);
		me->set_temp("apply/action/skill","xuanfeng-tui");
		if (COMBAT_D->do_attack(me, target, 0, 1))
		message_vision(HIR"$n连中几腿，$n连连后退，无暇出力！\n"NOR,me, target);
	}
	if ( target->query("qi") > 0 && me->is_fighting(target)) {
		message_vision(HIY"\n$N化掌为指，连点$n身前大穴！"NOR,me, target);
		me->set_temp("apply/action/skill","tanzhi-shentong");
		if (COMBAT_D->do_attack(me, target, 0, 3))
		message_vision(HIY"$N突然一指戳中$n太阳穴！$n摇摇晃晃，举步维艰。\n"NOR,me, target);
	}
	
	message_vision(HIW"\n$N飞影使完，手一晃将"+weapon->query("name")+HIW"拿回手中。\n"NOR,me, target);
	weapon->wield();
	me->add_temp("apply/pfm_attack", -240-lvl/10);
	me->add_temp("apply/damage", -350-lvl);
	me->add_temp("apply/pfm_damage", -180-lvl/4-i);
	me->start_busy(random(2));
	me->delete_temp("thd/feiying");
	me->delete_temp("apply/action");
	return 1;
}

string perform_name(){ return HIW"飞影"NOR; }

int help(object me)
{
        write(HIW"\n玉箫剑法之「飞影」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 2000 以上；
                最大内力 7500 以上；
                当前精力 2500 以上；
                玉箫剑法等级 350 以上；
                奇门八卦等级 180 以上；
                弹指神通等级 350 以上；
                兰花拂穴手等级 350 以上；
                落英神剑掌等级 350 以上；
                旋风扫叶腿等级 350 以上；
                碧海潮生功等级 350 以上；
                随波逐流等级 350 以上；
                手持箫。

HELP
        );
        return 1;
}

