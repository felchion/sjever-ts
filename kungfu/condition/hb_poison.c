
#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{
	if( !living(me))
		message("vision", me->name() + "冷得手脚都已发直。\n", environment(me), me);
	else {
		tell_object(me, HIB "忽然一阵刺骨的奇寒袭来，你中的寒冰绵掌的寒毒发作了！\n" NOR );
		message("vision", me->name() + "发起抖来，冷得全身缩成一团，手脚僵硬。\n", environment(me), me);
	}
	me->receive_damage("qi", 30+random(30));

	if(userp(me))
		me->receive_wound("jing", 30+random(30));
	else
		me->receive_wound("jing", 50+random(40));

	me->add_busy(2);
	me->set_temp("last_damage_from", "寒毒发作");
	me->apply_condition("hb_poison", duration - 1);
	if( duration < 2 ) return 0;
	return CND_CONTINUE & CND_NO_HEAL_UP;
}

string query_type(object me)
{
	return "poison";
}
