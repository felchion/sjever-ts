#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me,int duration)
{
	if(!environment(me)) return CND_CONTINUE; 
	me->receive_wound("qi",100);
	me->receive_wound("jing",100);
	me->add_busy(2);
	me->set_temp("last_damage_from","冰蚕毒发作");
	if( !living(me) ) message("vision", HIW+me->name() + HIW"的头上、衣服上、手脚上，都上一层薄薄的白霜。\n"NOR, environment(me), me);
	else {
		tell_object(me,HIB"你突然觉得食指尖上微微一痒，一股寒气优似冰箭，循着手臂，迅速无伦的射入胸膛。\n"NOR);
		tell_room(environment(me),HIW+me->name() + HIW"的头上、衣服上、手脚上，都上一层薄薄的白霜,冻得全身不停的哆嗦！\n"NOR,({me}));
		//message("vision", me->name() + "的头上、衣服上、手脚上，都上一层薄薄的白霜,冻得全身不停的哆嗦！\n", environment(me), me);
	}
	if(duration < 1) return 0;
	me->apply_condition("bingcan_poison",duration - 1);
	return CND_CONTINUE;
}

string query_type(object me)
{
        return "poison";
}
