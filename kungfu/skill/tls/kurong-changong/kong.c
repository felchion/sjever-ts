//kong for stop liumai-shenjian
//COOL@SJ

#include <ansi.h>

inherit F_CLEAN_UP;

string exert_name() {return HIW"空"NOR;}

int exert(object me)
{
             
	if(me->query_temp("start_6mai")) {
		me->delete_temp("start_6mai");
		remove_call_out("check_6mai");
		remove_call_out("checking");
		message_vision(HIG"$N屈指一收，手指垂下，体内鼓荡的六脉真气缓缓纳入丹田。\n\n"NOR, me);
	}
	else if(me->query_temp("pf_jiuchong") || me->query_temp("jiuchong_ok")) {
		remove_call_out("jiuchong_back");
		me->delete_temp("pf_jiuchong");
		remove_call_out("jiuchong_fight");
		me->delete_temp("jiuchong_ok");
		message_vision(HIG"$N手掌一分，体内遍布的九重天内劲缓缓纳入丹田。\n\n"NOR,  me);
	}
	else
		return notify_fail("你现在并没有运用特殊的内劲。\n");
	
	me->start_busy(random(2));
	
	return 1;
}
