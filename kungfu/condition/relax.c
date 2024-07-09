// condition relax.c

#include <ansi.h>
#include <login.h>

int update_condition(object me, int duration)
{
	mixed where;

	if (duration < 2) {
		message_vision(MAG"\n突然飘来一位绝色美女，将$N扶起离开了。\n\n"NOR, me);
		if (!stringp(where = me->query("relax_timeout_room")))
			if (me->query("enter_wuguan"))
				where = START_ROOM;
			else
				where = REVIVE_ROOM;
		me->delete("relax_timeout_room");
		me->move(where);
		me->set("startroom", where);
		me->save();
		message_vision(HIY "一位绝色美女带着$N走了过来！\n" NOR, me);
		tell_object(me, HIY "有人在耳边对你说道：这次就放了你，下次定不轻饶！\n" NOR);
		return 0;
	}

	if(!objectp(where = environment(me)))
		me->move(RELAX_ROOM);

	if (duration > 1 && base_name(where) != RELAX_ROOM) {
		message_vision(HIR "\n一位仙人走来，上下打量着$N，说道：“咦？桃花源的人什么时候跑这里来了？快跟我回去吧！”\n" NOR, me);
		message_vision("仙人带着$N，慢慢的离开了。\n", me);
//		me->set("eff_qi", 1);
//		me->set("qi", 1);
//		duration = me->query("rules")*90;
		me->move(RELAX_ROOM);
		message_vision(HIY "一位仙人带着$N走了过来！\n" NOR, me);
	}
	if (duration % 9 == 0) message_vision("一个声音说道：$N还有大约"+chinese_number(duration/3)+"分钟的禁闭时间。\n", me);
	me->apply_condition("relax", duration - 1);
	return 1;
}

string query_type(object me)
{
	return "relax";
}
