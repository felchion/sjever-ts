#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;
int update_condition(object me, int duration)
{
	int i;

	tell_object(me, HIR "你只觉得气血翻涌，浑身犹如千万支毒蛛在咬嗤，痛苦难忍！\n" NOR );
	tell_room(environment(me), HIR + me->name()+"面孔扭曲，弯下腰不住低哼，浑身不停的颤抖！\n" NOR, ({ me }));

	i = 200 + random(150);
	if ( !userp(me)) i *= 2;
	i -= me->query_con();

	if ( me->query_temp("qianzhu")){
		me->receive_wound("qi", i , "中千蛛万毒功");
		me->receive_wound("jing", i , "中千蛛万毒功");
		me->add_busy(1 + random(3));
	}
	else {
		me->receive_damage("qi", 15, "中千蛛万毒功");
		me->receive_wound("jing", 15, "中千蛛万毒功");
	}

	if( duration < 2 ) {
		me->delete_temp("qianzhu");
		return 0;
	}
	me->apply_condition("qzhu_poison", duration - 1);
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "poison";
}
