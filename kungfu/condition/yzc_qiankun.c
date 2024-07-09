#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{
	me->apply_condition("yzc_qiankun", duration - 1);
	if( duration < 1 ) return 0;
      
	tell_object(me, HIC "突然，侵入的丹田中的指劲乱窜，惊得你冷汗直冒！\n" NOR );
	tell_room(environment(me), HIC + me->name()+"突然一颤，脸上瞬间冒出许多冷汗！\n" NOR, ({ me }));  
	switch(duration) {
		case 1:	me->add_condition("no_exert", 5); 	break;
		case 3:	me->add_condition("no_perform", 5);	break;
		case 5:	me->add_condition("no_force", 5);	break;
		case 7:	me->add_busy(5); 			break;
		default:
			me->set_temp("last_damage_from", "一指禅内劲发作");
			me->receive_damage("qi", 50);
			me->receive_wound("qi", 50);
			me->add_busy(2);
			break;
	}
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "hurt";
}
