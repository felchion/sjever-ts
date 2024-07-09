// lifeforce.c 
#include <ansi.h>

string exert_name() {return MAG"内功疗伤"NOR;}

int exert(object me, object target)
{
	object where;
        int i;
        if( target == me )
                return notify_fail("你不能给自己疗伤。\n");

        if( !target )
                return notify_fail("你要用真气为谁疗伤？\n");
        where = environment(me);
	if ( !present(target,where) )
		return notify_fail("你要用真气为谁疗伤？\n");
        if( !living(target) || !target->query("can_speak"))
                return notify_fail("它不是活物，给它疗伤干什么？\n");

        if( me->is_fighting() || target->is_fighting())
                return notify_fail("战斗中无法运功疗伤！\n");

        if( me->is_busy() || target->is_busy())
                return notify_fail("忙乱中不能运功疗伤！\n");
        if( (int)me->query("max_neili") < 200 )
                return notify_fail("你的内力修为不够。\n");

        if ((int)me->query_skill("zixia-gong", 1) < 45)
                return notify_fail("你的紫霞功修为还不够。\n");

        if( (int)me->query("neili") < 200 )
                return notify_fail("你的真气不够。\n");

        if( (int)target->query_condition("hunyuan_hurt") == 0 )
                return notify_fail( target->name() + "并没有受到混元无极内劲的伤害！\n");

        if( (int)target->query("eff_qi") < (int)target->query("max_qi") / 5 )
                return notify_fail( target->name() +
                        "已经受伤过重，经受不起你的真气震荡！\n");

        message_vision(
                MAG "$N坐了下来运起内功，将手掌贴在$n背心，缓缓地将真气输入$n体内....\n\n"
                "过了不久，$N额头上冒出豆大的汗珠，$n吐出一口瘀血，脸色看起来红润多了。\n" NOR,
                me, target );

        target->receive_curing("qi", 10 + (int)me->query_skill("force")/3 );
        target->add("qi", 10 + (int)me->query_skill("force")/3 );
        if( (int)target->query("qi") > (int)target->query("eff_qi") ) {
        target->set("qi", (int)target->query("eff_qi")); }
        me->add("neili", -50);
        me->set("jiali", 0);
        me->start_busy(2+random(4));
        i = (target->query_condition("hunyuan_hurt") - 20);
        if( i >= 0 ) {
        target->apply_condition("hunyuan_hurt", i); }
        if( i < 0 ) {
        target->clear_condition("hunyuan_hurt"); }
        return 1;
}
