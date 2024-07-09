inherit F_SSERVER;
#include <ansi.h>
#include <combat_msg.h>

int perform(object me, object target)
{
	int damage, p;
	string msg;
        
	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !target->is_character() || !me->is_fighting(target) )
		return notify_fail("催心断肠只能对战斗中的对手使用。\n");

	if( !target->is_busy())
		return notify_fail("催心断肠只能对忙乱中的对手使用。\n");

	if( me->query_temp("weapon"))
		return notify_fail("你拿着武器！\n");   

	if( (int)me->query_skill("cuixin-zhang",1) < 180 )
		return notify_fail("你的掌法修为不足，使不出催心断肠。\n");

	if( (int)me->query("neili") < 1000 )
		return notify_fail("你现在内力太弱，不能打出催心断肠。\n");

	if( (int)me->query("jiali") < 100 )
		return notify_fail("你现在加力太弱，不能打出催心断肠。\n");

	if (me->query_skill_prepared("strike") != "cuixin-zhang"
	|| me->query_skill_mapped("parry") != "cuixin-zhang")
		return notify_fail("你现在无法使用催心断肠！\n");
                
	msg = HIR"\n$N趁着$n手忙脚乱之时，使出“催心断肠”！\n"NOR;
	damage = me->query_skill("strike");
	damage += random(me->query("jiali") * 3);
	damage *= 2;

	if(target->query("neili") < me->query("neili")) damage *= 2;

	if (damage > 2000) damage = 2000+random(500);  

	target->receive_damage("qi", damage, me);
	target->receive_wound("qi", damage/2, me);
	p = (int)target->query("qi")*100/(int)target->query("max_qi");
	msg += "结果重重的打在$n的胸口，$n登时鲜血狂喷。\n";
	msg += "( $n"+eff_status_msg(p)+" )\n"; 
        message_vision(msg, me, target);
	me->add("neili", -350);
	me->add("jingli", -150);
	me->start_busy(random(2));
	me->start_perform(3,"催心断肠");
	return 1;
}

string perform_name(){ return HIR"催心断肠"NOR; }

int help(object me)
{
        write(HIR"\n摧心掌之「催心断肠」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 1500 以上；
                当前精力 1000 以上；
                摧心掌等级 180 以上；
                激发掌法为摧心掌；
                激发招架为摧心掌；
                备掌法为摧心掌；
                且手无兵器。

HELP
        );
        return 1;
}
