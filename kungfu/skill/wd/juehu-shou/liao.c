// juehu.c 三阴绝户手「绝户」perform

#include <ansi.h>
inherit F_SSERVER;

int perform(object me, object target)
{
	string msg;

	if ( me->is_fighting())
		return notify_fail("战斗中疗伤？找死啊。\n");

	if( !target ) target = me;

	if ( target != me && !present(target, environment(me)))
		return notify_fail("没有这个人啊？\n");

	if( objectp(me->query_temp("weapon")) )
		return notify_fail("你必须空手才能使用「疗伤」！\n");  

	if( me->query_skill("juehu-shou", 1) < 200 )
		return notify_fail("你的虎爪绝户手不够娴熟，不会使用「疗伤」。\n");

	if( me->query_skill("yinyun-ziqi", 1) < 200 )
		return notify_fail("你的氤氲紫气不够高，不能用「疗伤」。\n");

	if ( me->query_skill("medicine", 1) < 120 )
		return notify_fail("你的本草术理等级不够。\n");

	if ( target->is_fighting())
		return notify_fail("战斗中疗伤？找死啊。\n");

	if ( ! target->query_condition("juehu_hurt")){
		if ( target == me)
			return notify_fail("你没有受「绝户」之伤啊？\n");
		else
			return notify_fail( target->name(1)+"并没有受「绝户」之伤。\n");
	}

	if ( me->query_skill_prepared("hand") != "juehu-shou"
	|| me->query_skill_mapped("parry") != "juehu-shou" )
		return notify_fail("你现在不能使用「疗伤」。\n");

	if( me->query("neili") < 1500  || me->query("max_neili") < 2000)
		return notify_fail("你现在内力太弱，不够用来「疗伤」。\n");

	if ( target == me) {
		msg = HIC"\n$N深吸一口气后缓缓坐下，化掌为指，封住自己的几处要穴，不久$N开始头冒白气，精神看起来好些了。\n"NOR;
		message_vision(msg, me);
	}
	else {
		msg = HIC"\n$N深吸一口气后缓缓坐下，化掌为指，封住$n的几处要穴，不久$n开始头冒白气，精神看起来好些了。\n"NOR;
		message_vision(msg, me, target);
	}
	target->add_condition("juehu_hurt", - me->query_skill("juehu-shou", 1)/10);

	me->add("neili", -500);
	me->add("max_neili", -5);
	me->start_busy(random(2));
	me->start_perform(3, "疗伤");
	return 1;
}

string perform_name(){ return HIC"疗伤"NOR; }

int help(object me)
{
        write(HIC"\n虎爪绝户手之「疗伤」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1500 以上；
                当前精力 2000 以上；
                虎爪绝户手等级 200 以上；
                氤氲紫气等级 200 以上；
                本草术理等级 120 以上；
                激发招架为虎爪绝户手；
                空手且对方或自己中了绝户伤。

HELP
        );
        return 1;
}
                                                                                                                                           