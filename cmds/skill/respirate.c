// respirate.c

#include <skill.h>

int respirating(object me);
int halt_respirate(object me, object who, string why);

int main(object me, string arg)
{
	int respirate_cost;
	object where = environment(me);

	seteuid(getuid());

	if (where->query("pigging"))
		return notify_fail("你还是专心拱猪吧！\n");

	if (me->is_busy())
		return notify_fail("你现在正忙着呢！\n");

	if (me->is_fighting())
		return notify_fail("战斗中吐纳, 太小看对手了。想找死！\n"); // changed by pishou

	if (where->query("sleep_room"))
		return notify_fail("卧室不能吐纳，会影响别人休息。\n");

	if (where->query("no_fight"))
		return notify_fail("这里不准战斗，也不准吐纳。\n");

	if (me->query_condition("no_force"))
		return notify_fail("你感到体内气息乱窜，无法聚气吐纳！\n");

	if (!stringp(me->query_skill_mapped("force")))
		return notify_fail("你必须先用 enable 选择你要用的特殊内功。\n");

	if (!arg || !sscanf(arg, "%d", respirate_cost))
		return notify_fail("你要花多少精修行？\n");

	if (respirate_cost < 10)
		return notify_fail("你的内功修行还没有高到能如此精确控制！\n");

	if (me->query("jing") < respirate_cost)
		return notify_fail("你现在精不足，无法修行精力！\n");

	if (me->query("jingli") > (me->query("eff_jingli")+me->query_temp("apply/jingli"))*2)
		return notify_fail("你现在全身积满精力，无法再继续修行了。\n");

	if (me->query("qi") * 100 / (me->query("max_qi")+me->query_temp("apply/qi")) < 70)
		return notify_fail("你现在身体状况太差了，无法集中精神！\n");

	write("你闭上眼睛开始吐纳。\n");
	me->set_temp("pending/respirate", 1);
//	tell_room(environment(me),
//		me->name() + "盘膝坐下，开始吐纳炼精。\n", ({ me }));
	me->set_temp("respirate_cost", respirate_cost);
	me->start_busy((: respirating :), (: halt_respirate :));
	return 1;
}

int respirating(object me)
{
	int respirate_cost = me->query_temp("respirate_cost");
	int jingli_gain = 1 + me->query_skill("force", 1) / 10
		* (1 + me->query_skill("force") / 100);
	int max;
	
	if (me->query("age") < 20)
		jingli_gain += jingli_gain * (20 - me->query("age")) / 10;
		
	jingli_gain = jingli_gain * 3;
	jingli_gain = jingli_gain / 2;//加快dazuo tuna速度 by ciwei
	
	if (jingli_gain > respirate_cost)
		jingli_gain = respirate_cost;
	me->add("jingli", jingli_gain);
	me->set_temp("respirate_cost", respirate_cost -= jingli_gain);
	me->receive_damage("jing", jingli_gain);

	if (respirate_cost > 0)
		return 1;

	me->delete_temp("pending/respirate");
	tell_object(me, "你吐纳完毕，睁开双眼，站了起来。\n");

	jingli_gain = 0;
	max = (me->query_skill("force")-me->query_temp("apply/force")) * 8;
	while (me->query("jingli") >= (me->query("eff_jingli")+me->query_temp("apply/jingli")) * 2) {
		if (me->query("max_jingli") >= max) {
			tell_object(me, "你的精力修为已经达到了你内功所能控制的极限。\n");
			me->set("jingli", (me->query("eff_jingli")+me->query_temp("apply/jingli")) * 2);
			break;
		}
		if(me->query("env/积蓄"))
		{
			tell_object(me,"你的精力修为已经达到达到圆满之境。\n");
			me->set("jingli", (me->query("eff_jingli")+me->query_temp("apply/jingli")) * 2);
			break;			
		}
		me->add("max_jingli", 1);
		me->add("jingli", 2-me->add("eff_jingli", 1)-me->query_temp("apply/jingli"));
		jingli_gain++;
	}
	if (jingli_gain)
		tell_object(me, "你的精力修为增加了"+chinese_number(jingli_gain)+"点！\n");
	return 0;
}

int halt_respirate(object me, object who, string how)
{
	if (me->query("jingli")	> (me->query("eff_jingli")+me->query_temp("apply/jingli")) * 2)
	me->set("jingli", (me->query("eff_jingli")+me->query_temp("apply/jingli")) * 2);
	me->delete_temp("pending/respirate");
	message_vision("$N猛吸几口大气，站了起来。\n", me);
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式 : respirate|tuna [<耗费「精」的量>]

打坐修行，利用「炼精化气，炼气化神，炼神还虚」的方法将你的精
转变成精力。

See also: exercise
HELP
	);
	return 1;
}
