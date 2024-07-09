// mj_jail.c
#include <ansi.h>
#include <login.h>

int update_condition(object me, int duration)
{
	object where;
	if (duration < 2) {
		me->move("/d/mingjiao/shaqiu2");
		message("vision",HIY "你听到一声：“快滚，下次别让我再见到你”。原来是一个全身糟透了的家伙给扔到了沙漠。\n"NOR, environment(me), me);
		tell_object(me, HIY "只觉被人连抬带拖，你昏昏沉沉地被扔到了沙漠之中！\n" NOR);
		me->set("startroom", START_ROOM);
		return 0;
        }

	where = environment(me);
	if ( !wizardp(me) && base_name(where)!="/d/mingjiao/jianyu") {
		message_vision(HIR "突然冲出几名明教高手，将$N乱棒打晕......\n" NOR, me);
		me->unconcious();
		message_vision("明教高手扛着晕死过去的$N，扬长而去。\n", me);
		me->move("/d/mingjiao/jianyu");
		me->set("eff_qi", 1);
		me->set("qi", 1);
		duration = 120;
	}
	me->apply_condition("mj_jail", duration - 1);
	return 1;
}

string query_type(object me)
{
	return "jail";
}
