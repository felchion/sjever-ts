// gmove.c  转移帮派
// by augx@sj 4/15/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object me)
{
	write(@HELP
指令格式 :
gmove [站点英文id]
把帮派控制权转移到指定站点，转移操作每天早晨系统进行帮派检查与同步时执行。
只有帮主和副帮主有权利执行此操作。站点英文id请用“mudlist”命令查看，并注
意大小写。

HELP
	);
	return 1;
}


int main(object me, string arg)
{
	string gid;

	if( !arg ) return help(me);
	if( !(gid=me->query("group/id")) )
		return notify_fail("你还没有创建自己的帮派呢。\n");
	if( me->query("group/class") > 2 )
		return notify_fail("你的等级不够。\n");
	if( !GROUP_D->site_now( gid ) )
		return notify_fail( GROUP_D->get_last_error() );

	if( !GROUP_D->set_site_next(gid,arg) )
		return notify_fail( GROUP_D->get_last_error() );

	write("你决定把"+GROUP_D->get_group_name(gid)+"的控制权转移到"+arg+"。\n");
	return 1;
}
