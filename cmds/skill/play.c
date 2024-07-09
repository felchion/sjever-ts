// play.c

#include <ansi.h>;

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object env;

	if (!objectp(present("wei qi", me))
	&& !objectp(present("gu zheng", me)))
		return 0;

	if (!arg ) return notify_fail("你要做什么？\n");

	if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");

	env = environment(me);
	if (env->query("sleep_room"))
		return notify_fail("卧室不能修练，会影响别人休息。\n");

	if (env->query("pigging"))
		return notify_fail("你还是专心拱猪吧！\n");

	if (env->query("pending"))
		return notify_fail("此地不宜修练！\n");

	if (env->query("no_fight"))
		return notify_fail("这里不准战斗，也不准修练。\n");

	if (arg == "gu zheng" && present("gu zheng", me)) {
		if (me->query("qi") < 20 || me->query("jing") < 30)
			return notify_fail("你太累了，还是先休息会吧。\n");
		if (random(10) > 5) {
			message_vision("$N摆出一张古筝，铮铮琮琮弹了几声只听得曲调杂乱无章，宛如鬼哭狼号。\n", me);
			me->receive_damage("jing", random(10));
			me->receive_damage("qi", random(10));
			return 1;
		}
		me->receive_damage("jing", 20);
		me->receive_damage("qi", 5);
		me->add_busy(random(2));
		if (me->query_skill("art", 1) < 120)
			me->improve_skill("art", me->query_int()/4 + me->query_skill("art", 1)/10);
		write("你摆出一张古筝，调了调弦，弹了起来，你只觉得连日来烦燥的心情好了许多。\n");
		tell_room(env, "只见" + me->name() + "雅兴大发，竟弹起曲子来，你只觉得时而金戈铁马、大漠平沙……\n"
				"时而小桥流水、几户人家……万般气象，你心情顿时宁静了许多。\n", me);
		return 1;
	}
	if (present("wei qi", me) && arg == "wei qi") {
		if (me->query("qi") < 20 || me->query("jing") < 30)
			return notify_fail("你太累了，还是先休息会把。\n");
		if (random(10) > 5) {
			message_vision("$N摆出一张棋盘，想研究围棋，却总觉得心烦意乱，定不下神来。\n", me);
			me->receive_damage("jing", random(10));
			me->receive_damage("qi", random(10));
			return 1;
		}
		me->receive_damage("jing", 20);
		me->receive_damage("qi", 5);
		me->add_busy(random(2));
		if (me->query_skill("art",1) < 120)
			me->improve_skill("art", me->query_int()/4 + me->query_skill("art", 1)/10);
		write("你摆出一张棋盘，研究起围棋来，只觉得心平气和，很快就仿佛入定了。\n");
		tell_room(env, "只见" + me->name() +"雅兴大发，竟打起棋谱来，对外界仿佛不闻不问，一副莫测高深的模样。\n", me);
		return 1;
	}
	return notify_fail("你想做什么？\n");
}
