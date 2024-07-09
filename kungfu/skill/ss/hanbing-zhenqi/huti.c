// huti.c

#include <ansi.h>
int query_ap() {return 2;}
void check_fight(object me, int count);
int exert(object me, object target)
{
	string msg;

	if (!me->is_fighting())
		return notify_fail("你只能在战斗中使用「护体诀」。\n");
		
	if (me->query_temp("hbzq/huti"))
		return notify_fail("你已经在使用「护体」了。\n");

	if (me->query_skill("hanbing-zhenqi",1) < 100)
		return notify_fail("你的「寒冰真气」功力不够，无法施用「护体」。\n");

	if (me->query("max_neili") < 1000)
		return notify_fail("你的内力不够，无法使用「护体」。\n");

	if (me->query("neili") < 500)
		return notify_fail("你的真气不够，无法使用「护体」。\n");

	me->add("neili", - 250);
	msg = HIB"$N默念寒冰决，周身寒气越来越浓，隐隐把自己整个人裹了起来！\n"NOR;

	me->add_temp("apply/pfm_parry", 180);
	me->set_temp("hbzq/huti", 1);

  call_out("check_fight", 1, me, 10+me->query_skill("hanbing-zhenqi",1)/10);
	message_vision(msg, me);
	return 1;
}

void remove_effect(object me);

void check_fight(object me, int count){
	if (!me) return;
	if( !me->is_fighting() || count < 1 ){
		remove_effect(me);
		return;
	}
	call_out("check_fight", 1, me, count-1);
}

void remove_effect(object me)
{
	if (!me || !me->query_temp("hbzq/huti"))
		return;
	me->add_temp("apply/pfm_parry", -180);
	me->delete_temp("hbzq/huti");
	if (living(me) && ! me->is_ghost())
		message_vision(HIW"$N周身的寒气散尽，不再能起到保护的作用了。\n"NOR,me);
}

string exert_name()
{
	return HIB"护体诀"NOR;
}

int help(object me)
{
	write(HIB"\n寒冰真气之「护体诀」：\n\n"NOR);
	write(@HELP
	要求：	寒冰真气等级 100 以上；
		最大内力 1000 以上；
		当前内力 500 以上。

HELP
	);
	return 1;
}
