// by snowman@SJ 18/02/2000
// YUJ 2001-10-29

#include <ansi.h>

inherit F_CLEAN_UP;
#define HEAD "/clone/misc/head"

int main(object me, string arg)
{
	object target, ob, weapon, murderer;

	if (!arg) return notify_fail("你要斩什么？\n"); 
 
	if (me->is_busy() || me->is_fighting())
		return notify_fail("你现在正忙着呢。\n"); 

	if( !target = present(arg, environment(me)) ) {
		if( !target = present(arg, me) ) 
			return notify_fail("找不到这个东西。\n");
	}

	if( !target->is_corpse() ) 
		return notify_fail("看清楚点，那不是尸体。\n");
	if (target->query("race") != "人类")
		return notify_fail("不会吧，你对动物的尸体也感兴趣？\n");
	if (target->query("no_head"))
		return notify_fail("那具尸体已经没有首级了。\n");
	if (!target->is_character())
		return notify_fail("那具尸体已经腐烂了。\n");
	murderer = target->query("kill_by");
	if (objectp(murderer) && murderer != me)
		return notify_fail("乱切别人杀的人干嘛啊？\n"); 
	if (!objectp(weapon = me->query_temp("weapon")))
		return notify_fail("你得用件锋利的器具才能切下这尸体的头来。\n");

	if (weapon->query("flag") != 4)
		return notify_fail("你手上这件兵器无锋无刃，如何能切下这尸体的头来？\n");

	message_vision("\n$N扬起"+weapon->name()+"，对准$n的脖子比了比，猛斩了下去！\n", me, target);

	ob = new(HEAD);
	ob->set_name( target->query("victim_name")+"的首级" , ({ "shouji", "head" }) );
	ob->set("long", "这是一颗"+target->query("victim_name")+"的首级。\n");

	if (objectp(murderer)) {
//		target->set("kill_by", target->query("kill_by"));
		ob->set("kill_by", murderer);
		target->delete("kill_by");
	} else
		ob->set("kill_by", me);

	ob->set("victim_id", target->query("victim_id"));
	target->delete("victim_id");
	ob->set("victim_name", target->query("victim_name"));
	ob->set("victim_user", target->query("victim_user"));

	message_vision("只听“咔”的一声，$N将$n斩了下来，提在手中。\n随后$N又飞起一脚把尸体踢飞。\n", me, ob);

	ob->move(me);
	destruct(target);
	me->add_busy(1);
	return 1;
}      

int help(object me)
{
	write(@HELP
指令格式 : qie <尸体>
 
这个指令可以让你把尸体的头部切下来。
 
HELP
	);
	return 1;
}
