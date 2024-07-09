// declare war
// by augx@sj 3/28/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object me)
{
	write(@HELP
指令格式：declare war to [帮派id]
向某个帮派宣战。帮主或副帮主可以独立宣战，堂主需要两人或以上才能宣战。
如果对方帮派的首领有五人，则对方帮派必须有两名或以上首领在线，才能对之
宣战；如果对方帮派首领少于五人，则只要对方帮派有一名首领在线，就可对之
宣战，宣战方必须建有总坛且存款大于一千锭黄金，帮派总人数不少于15人。
HELP );
	return 1;
}

int main(object me, string arg)
{
	string gid;
	int i,j;
	mixed res;

	if( !arg || sscanf(arg,"%s to %s",arg,gid)<2 ) return help(me);
	if( arg != "war" ) return help(me);
	if( !me->query("group/id") )
		return notify_fail("你还没有加入帮派，瞎喊什么啊？\n");
	if( me->query("group/class") > 3 )
		return notify_fail("你还不够资格宣战。\n");
	if( me->query("group/id") == gid )
		return notify_fail("向自己的帮派宣战？\n");
	if( !GROUP_D->site_now(me->query("group/id")) )
		return notify_fail( GROUP_D->get_last_error() );
	if( !GROUP_D->site_now(gid) )
		return notify_fail( GROUP_D->get_last_error() );

	if ( GROUP_D->get_group(me->query("group/id"), 0)["money"] < 1000)
		return notify_fail("你没有设立总坛，无法宣战。\n");

	if ( GROUP_D->get_group_members(me->query("group/id")) < 15 )
		return notify_fail("你帮派里面的人数不够，无法宣战。\n");

	if( GROUP_D->get_group(gid,1)["target"] != me->query("group/id") ) {
		i = GROUP_D->query_master_count(gid);
		j = GROUP_D->query_master_count(gid,1);
		if ((i>=5 && j<2) || j<1)
			return notify_fail("对方的领导人都不在，你宣战也没有用啊。\n");
	}

	if( stringp(res=GROUP_D->declare_war(me,gid)) )
		return notify_fail(res);

	return 1;
}
