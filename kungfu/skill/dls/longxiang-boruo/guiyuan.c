
#include <ansi.h>

int exert(object me, object target)
{
	object tar;

	if( !target)
		return notify_fail("你要对谁使用搜魄归元大法？\n");

	if ((int)me->query_skill("longxiang-boruo", 1) < 140)
		return notify_fail("只有修炼到龙象般若功第七层之后，才能施展聚魄归元。\n");

	if((int)me->query("max_neili") < 2000)
		return notify_fail("你的内力修为太差，多多打坐吧。\n");

	if( (int)me->query("neili") < 1000 ) 
		return notify_fail("你的真气不够。\n"); 

	if( me->query("jingli") < 500 )
		return notify_fail("你的精力不够！\n");

	if ( me->is_fighting())
		return notify_fail("战斗中使用搜魄归元大法，找死啊？\n");

	if( !objectp(target) || target == me || ! objectp(tar=present(target, environment(me))) )
		return notify_fail("你想要帮谁凝魄归元？\n");

	if( tar->is_corpse() )
		return notify_fail("来不及了，只有活人才能救醒。\n");

	if ( living(tar))
		return notify_fail(tar->name(1)+"好好的，你对他使用什么搜魄归元大法啊？\n");

	me->add("jingli", -50);
	me->add("neili", -500);
	message_vision( HIY "$N一手放在$n的天灵盖上，一手贴在$n的後心，闭上眼睛缓缓低吟 ...\n" NOR, me, target );

	if( random(me->query("max_jingli")) > 500 ) {
		tar->revive();
		me->add("eff_jing", -100);
		me->receive_damage("qi", 300);
		me->add_busy(3);
	}
        else
		me->unconcious();

	me->start_exert(3, "搜魄归元");
	return 1;
}

string exert_name(){ return HIY"搜魄归元"NOR; }

int help(object me)
{
        write(HIY"\n龙象般若功之「搜魄归元」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 2000 以上；
                当前精力 500 以上；
                龙象般若功等级 140 以上。

HELP
        );
        return 1;
}
