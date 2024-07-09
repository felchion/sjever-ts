// yield.c
// Created by snowman@SJ 1998

#include <room.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	if (!arg) {
		if(me->query_temp("combat_yield"))
		arg = "no";
		else
		arg = "yes";
	}

	if (arg == "yes") {
		if (me->is_busy())
		return notify_fail("你正忙着呢。\n");
		if (me->is_perform())
		return notify_fail("你正在使用"+me->query_perform()+"，暂时无法打不还手。\n");
		me->set_temp("combat_yield", 1);
		//	me->remove_all_enemy();
		tell_object(me, "你决定打架时打不还手。\n");
	}
	else if( arg == "no"){
		me->delete_temp("combat_yield");
		tell_object(me, "你决定打架时还手反击。\n");
	}
	else return notify_fail("你决定打架还手(yield no)还是不还手(yield yes)？\n");

	return 1;
}

int help(object me)
{
	write(@HELP
指令格式 : yield <yes | no>
 
这个指令可以让你决定打架时是否还手反击。
 
HELP
    );
    return 1;
}
