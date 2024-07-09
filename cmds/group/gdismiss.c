// gdismiss.c  解散帮派
// by augx@sj 4/11/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object me)
{
	write(@HELP
指令格式 :
gdimiss
解散帮派，只有帮派的最高首领可以使用这个命令。

gdimiss [帮派id]
巫师专用命令，解散指定帮派。
HELP
	);
	return 1;
}

int main(object me, string arg)
{
	string gid;
	mapping mp;

	if( !me->query("group") && !wizardp(me) )
		return notify_fail("你还没有创建自己的帮派呢。\n");
	if( me->query("group/class")!=1 && !wizardp(me) )
		return notify_fail("只有帮派的首领和巫师才有权利解散帮派。\n");
	if( wizardp(me) && arg ) gid = arg; else gid = me->query("group/id");
	if( !GROUP_D->site_now(gid) )
		return notify_fail( GROUP_D->get_last_error() );
	if( !(mp=GROUP_D->get_group(gid,1)) )
		return notify_fail("没有这个帮派。\n");
	if( mp["war"] > 1 )
		return notify_fail("你们的帮派正在战争中呢。\n");

	write("你确定要解散你的帮派吗？[y/n] ");
	input_to("confirm_dismiss",me,gid);

	return 1;
}

protected void confirm_dismiss(string arg,object me,string gid)
{
	if( arg[0]!='y' && arg[0]!='Y' ) return;
	
	GROUP_D->set_res("帮主主动解散");
	GROUP_D->delete_group(gid);
	tell_object(me,"你解散了自己的帮派。\n");
	log_file("group/group",sprintf("dismiss:dismiss  :%-8s:%s(%s)",gid,me->name(),me->query("id")),me);
}
