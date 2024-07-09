// gjoin.c
// by augx@sj 4/4/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object me)
{
	write(@HELP
指令格式 :
gjoin [帮派id]
请求加入某个帮派。
HELP
	);
	return 1;
}

int main(object me, string arg)
{
	if( !arg ) return help(me);
	if( me->query("group/id") )
		return notify_fail("你已经加入一个帮派了。\n");
	if( me->query("combat_exp") < 100000 )
		return notify_fail("你的资历不够，还是等一阵再去混帮派吧。\n");
	if( !GROUP_D->site_now(arg) )
		return notify_fail( GROUP_D->get_last_error() );

	me->set_temp("group/join",arg);
	message_vision("$N要求加入"+GROUP_D->get_group_name(arg)+"。\n",me);
	return 1;
}
