// ally.c
// by augx@sj 4/3/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object me)
{
	write(@HELP
指令格式 :
ally request [帮派id]
请求和某个帮派结盟，只有处于战争或战争准备状态下的帮派，才能请求结盟。

ally answer [帮派id]
同意和某个帮派结盟，需要先有别的帮派发出结盟请求，才能使用此命令。

ally renege [帮派id]
取消和某个帮派的结盟关系，或取消对某个帮派发出的结盟请求。
HELP
	);
	return 1;
}

int main(object me, string arg)
{
	string type,gid1,gid2;

	if( !arg || sscanf(arg,"%s %s",type,gid2)<2 || (type!="request" && type!="answer" && type!="renege") )
		return help(me);
	if( !(gid1=me->query("group/id")) )
		return notify_fail("你并没有加入帮会，结盟干什么呀？\n");
	if( me->query("group/class") > 3 )
		return notify_fail("你的等级不够，不能和别的帮派结盟。\n");

	if( !GROUP_D->do_friend(gid1,gid2,type) )
		return notify_fail( GROUP_D->get_last_error() );

	switch( type ) {
		case "request":
			GROUP_D->tell_group_master(gid1,"你们提出与"+GROUP_D->get_group_name(gid2)+"结盟。\n",5);
			GROUP_D->tell_group_master(gid2,GROUP_D->get_group_name(gid1)+"提出与你们的帮派结盟的请求。\n",5);
			break;
		case "answer":
			GROUP_D->tell_group_master(gid1,"你们已经答应与"+GROUP_D->get_group_name(gid2)+"结盟。\n",5);
			GROUP_D->tell_group_master(gid2,GROUP_D->get_group_name(gid1)+"已经答应与你们结盟。\n",5);
			break;
		case "renege":
			GROUP_D->tell_group_master(gid1,"你们已经解除了与"+GROUP_D->get_group_name(gid2)+"的盟约。\n",5);
			GROUP_D->tell_group_master(gid2,GROUP_D->get_group_name(gid1)+"已经解除了与你们的盟约。\n",5);
			break;
	}

	return 1;
}
