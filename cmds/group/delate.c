// delate.c
// by augx@sj 4/4/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object me)
{
	write(@HELP
指令格式 : delate/tanhe
弹劾自己帮派的帮主。帮主不在线时也能弹劾。帮主被弹劾后将退出本帮派，
但没有经验损失。如果你们的帮派有五名首领，则必须得到三人同意；如果你
们的帮派有四或三名首领，则必须得到二人同意；如果你们的帮派只有两名首
领，则只需要一人的同意。
HELP
	);
	return 1;
}

int main(object me, string arg)
{
	if( !me->query("group/id") )
		return notify_fail("你还没有加入帮派呢。\n");
	if( me->query("group/class") > 3 )
		return notify_fail("你的等级不够。\n");
	if( me->query("group/class") == 1 )
		return notify_fail("自己弹劾自己？\n");

	GROUP_D->start_vote(me,me->query("group/id"),me->name()+"提出要弹劾你们目前的"+GROUP_D->get_member_class(me->query("group/id"),1)+"，请问是否同意？(y/n) ","delate",60);
	tell_object(me,"你已经发出了弹劾首领的提议，但还需要等待帮会中其他领导人的回应。\n");

	return 1;
}
