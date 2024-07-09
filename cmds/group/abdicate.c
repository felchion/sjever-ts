// abdicate.c
// by augx@sj 4/10/2002

#include <ansi.h>
inherit F_CLEAN_UP;
#include <group.h>

int help(object me)
{
	write(@HELP
指令格式 : abdicate/chanrang to [玩家id]
把自己的帮主位置禅让给自己帮派里的其他玩家。
禅让的对象必须符合帮主的经验要求。
禅让后，原首领没有经验损失。
HELP
	);
	return 1;
}

int main(object me, string arg)
{
	string gid,str;
	object ob;

	if( !arg || sscanf(arg,"to %s",arg) < 1 ) return help(me);
	if( !(gid=me->query("group/id")) )
		return notify_fail("你还没有加入帮派呢。\n");
	str = GROUP_D->get_member_class(gid,1);
	if( me->query("group/class") != 1 )
		return notify_fail("你又不是"+str+"，如何禅位？\n");
	if( me->query("id") == arg )
		return notify_fail("自己禅位给自己？\n");
	if( !arg || !(ob=find_player(arg)) )
		return notify_fail("找不到要禅让的人啊。\n");
	if( ob->query("group/id") != gid )
		return notify_fail(ob->name()+"并不是你们"+GROUP_D->query_group_name(gid)+"的成员啊。\n");
	if( ob->query("combat_exp") < EXP_REQUIRE )
		return notify_fail(ob->name()+"的资历不够，禅位于他恐怕难以伏众。\n");

	if( !GROUP_D->set_group_master(gid,arg,0) )
		return notify_fail( GROUP_D->get_last_error() );
	if( !GROUP_D->set_group_master(gid,me->query("id"),0) )
		return notify_fail( GROUP_D->get_last_error() );
	if( !GROUP_D->set_group_master(gid,arg,1) )
		return notify_fail( GROUP_D->get_last_error() );
	ob->set("group/class",1);
	me->set("group/class",4);
	me->save();
	ob->save();
	GROUP_D->check_user_group(me);
	GROUP_D->check_user_group(ob);

	tell_object(me,"你决定把"+str+"之位禅让给"+ob->name()+"。\n");
	tell_object(ob,me->name()+"决定把"+str+"之位禅让给你。\n");
	CHANNEL_D->do_channel(this_object(),"rumor",HIM"据说"+me->name()+HIM+"已经把"+GROUP_D->get_group_name(gid)+HIM"的"+str+HIM"之位禅让给"+ob->name()+HIM"了。"NOR);
	log_file("group/manage",sprintf("abdicate:%s:%s(%s):%s(%s)",gid,me->name(),me->query("id"),ob->name(),ob->query("id")),me);

	return 1;
}
