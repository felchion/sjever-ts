// exercise.c
// snowman@SJ 15/12/1998
// modify by snowman@SJ 01/03/2000

#include <skill.h>

int exercising(object me);
int halt_exercise(object me);

int main(object me, string arg)
{
	string force;
	mapping msg;
	int exercise_cost, max;
	object where;

	max = (int)me->query("max_neili");
	seteuid(getuid());
	where = environment(me);

	if (where->query("pigging"))
		return notify_fail("你还是专心拱猪吧！\n");

	if (where->query("pending"))
		return notify_fail("此地不宜修练！\n");

	if (me->is_busy())
		return notify_fail("你现在正忙着呢。\n");

	if (me->is_exert())
		return notify_fail("你刚施用过内功，不能马上打坐。\n");

	if( me->is_fighting() )
		return notify_fail("战斗中不能练内功，会走火入魔。\n");

	if (where->query("sleep_room"))
		return notify_fail("卧室不能打坐，会影响别人休息。\n");

	if (where->query("no_fight"))
		return notify_fail("这里不准战斗，也不准打坐。\n");

	if (me->query_condition("no_force"))
		return notify_fail("你感到体内气息乱窜，无法聚气打坐！\n");

	if (!(force = me->query_skill_mapped("force")))
		return notify_fail("你必须先用 enable 选择你要用的特殊内功。\n");

	if (!arg || !sscanf(arg, "%d", exercise_cost))
		return notify_fail("你要花多少气练功？\n");

	if ((me->query("max_qi")+me->query_temp("apply/qi")) > 1000
	&& exercise_cost < (me->query("max_qi")+me->query_temp("apply/qi"))/5)
		return notify_fail("你至少需要"+chinese_number((me->query("max_qi")+me->query_temp("apply/qi"))/5)+"点的气来打坐！\n");

	if (exercise_cost < 10)
		return notify_fail("你至少需要十点的气来打坐！\n");

	if (me->query("qi") < exercise_cost )
		return notify_fail("你没有那么多的气来产生内息运行全身经脉。\n");

	if (me->query("jing") * 100 / (me->query("max_jing")+me->query_temp("apply/jing")) < 70 )
		return notify_fail("你现在精不够，无法控制内息的流动！\n");

	if (me->query("neili") > (me->query("max_neili")+me->query_temp("apply/neili"))*2)
		return notify_fail("你现在全身积满内力，无法再继续打坐了。\n");

	msg = SKILL_D(force)->exercise_msg(me);
	if (!msg || undefinedp(msg["start_my_msg"]))
		write("你坐下来运气用功，一股内息开始在体内流动。\n");
	else
		write(msg["start_my_msg"]);

	me->set_temp("pending/exercise", 1);
	me->set_temp("exercise_cost", exercise_cost);
/*
	if (!msg || undefinedp(msg["start_other_msg"]))
		tell_room(environment(me),
			me->name() + "盘膝坐下，开始运功打坐。\n", ({ me }));
	else
		tell_room(environment(me),
			msg["start_other_msg"], ({ me }));
*/
	me->start_busy((: exercising :), (:halt_exercise:));
	me->start_exert(1);
	return 1;
}

int exercising(object me)
{
	string force;
	mapping msg;
	int exercise_cost = me->query_temp("exercise_cost");
	int neili_gain = 1 + me->query_skill("force") / 15
		* (1 + (int)me->query_skill("force") / 60);
	int max;

//	if( me->query("age") > 20 )
//	neili_gain *= 20 / me->query("age");
	if (me->query("age") < 20)
		neili_gain += neili_gain * (20 - me->query("age")) / 10;
		
	neili_gain = neili_gain * 3;
	neili_gain = neili_gain / 2;//加快dazuo tuna速度 by ciwei

	if (neili_gain > exercise_cost)
		neili_gain = exercise_cost;

	me->receive_damage("qi", neili_gain, "走火入魔");
	me->add("neili", neili_gain);
	me->set_temp("exercise_cost", exercise_cost -= neili_gain);

	force = me->query_skill_mapped("force");
	if (force) msg = SKILL_D(force)->exercise_msg(me);
	if (exercise_cost > 0) {
		if (msg && !undefinedp(msg["exec_my_msg"]))
			tell_object(me, msg["exec_my_msg"]);
//		if (!random(3) && msg && !undefinedp(msg["exec_other_msg"]))
//			tell_room(environment(me), msg["exec_other_msg"], me);
		return 1;
	}

	me->delete_temp("pending/exercise");
	if (!msg || undefinedp(msg["end_my_msg"]))
		tell_object(me, "你运功完毕，站了起来。\n");
	else
		tell_object(me, msg["end_my_msg"]);
/*
	if (!msg || undefinedp(msg["end_other_msg"]))
		tell_room(environment(me),
			me->name()+"运功完毕，站了起来。\n", ({me}));
	else
		tell_room(environment(me),
			msg["end_other_msg"], ({me}));
*/
	neili_gain = 0;
	max = (me->query_skill("force")-me->query_temp("apply/force")) * 8 + me->query("combat_exp") / 1000;
	while (me->query("neili") >= (me->query("max_neili")+me->query_temp("apply/neili")) * 2) {
		if (me->query("max_neili") >= max) {
			tell_object(me, "你的内力修为已经无法靠打坐来提升了。\n");
			me->set("neili", (me->query("max_neili")+me->query_temp("apply/neili")) * 2);
			break;
		}
		if(me->query("env/积蓄"))
		{
			tell_object(me,"你的内力修为已经达到圆满之境。\n");
			me->set("neili", (me->query("max_neili")+me->query_temp("apply/neili")) * 2);
			break;			
		}
		me->add("neili", 2-me->add("max_neili", 1)-me->query_temp("apply/neili"));
		neili_gain++;
	}
	if (neili_gain)
		tell_object(me, "你的内力修为增加了"+chinese_number(neili_gain)+"点！\n");
	return 0;
}

int halt_exercise(object me)
{
	string force;
	mapping msg;

	if (me->query("neili") > (me->query("max_neili")+me->query_temp("apply/neili")) * 2)
		me->set("neili", (me->query("max_neili")+me->query_temp("apply/neili")) * 2);
	me->delete_temp("pending/exercise");
	force = me->query_skill_mapped("force");
	if (force) msg = SKILL_D(force)->exercise_msg(me);
	if (!msg || undefinedp(msg["halt_msg"]))
		message_vision(
			"$N把正在运行的真气强行压回丹田，站了起来。\n", me);
	else
		message_vision(msg["halt_msg"], me);

	if (me->is_fighting())
		me->add_busy(2);
	else
		me->add_busy(random(2));
	notify_fail("");
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式 : exercise|dazuo [<耗费「气」的量> 必须不少于 10]

运气练功，控制体内的气在各经脉间流动，藉以训练人体肌肉骨骼的耐
力、爆发力，并且用内力的形式将能量储备下来。
HELP
	);
	return 1;
}
