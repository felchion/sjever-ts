#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;
int update_condition(object me, int duration)
{
	if( !living(me))
		message("vision", HIB+ me->name() + "痛苦地哼了一声。\n", environment(me), me);
	else {
		tell_object(me, HIB "你突然觉得一股恶臭从腹中升起，不禁大呕起来！\n" NOR );
		tell_room(environment(me), HIB + me->name()+"脸上浮着一层黑气，脚步蹒跚，突然吐出一堆秽物！\n" NOR, ({ me }));
	}
	me->receive_wound("qi", me->query("max_qi")/5, "腐尸毒发作");
	me->receive_wound("jing", me->query("max_jing")/5, "腐尸毒发作");
	me->add_busy(1);
	if( duration < 1 ) return 0;
	me->apply_condition("fs_poison", duration - 1); 
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "poison";
}
 