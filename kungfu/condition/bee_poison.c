//bee_poison.c 玉蜂毒
#include <ansi.h>
#include <condition.h>
inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{
	object where=environment(me);
	tell_object(me, MAG"突然你只感麻痒难当，似乎五脏六腑也在发痒！\n" NOR );
	tell_room(environment(me), HIR+me->name()+"已然立足不定，倒在"+where->query("short")+HIR"的地上滚来滚去，大声呼叫！\n" NOR,({ me }));
	me->receive_damage("qi", 50+random(30));
	me->receive_wound("jing", 30+random(30));
	me->add_busy(2);
	me->set_temp("last_damage_from", "中玉蜂毒发作");
	me->apply_condition("bee_poison", duration - 1);
	if( duration < 1 ) return 0;
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "poison";
}
