// grant.c
// by augx@sj 4/4/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object me)
{
	write(@HELP
指令格式 :
grant [玩家id] [等级]
设置玩家在帮派中的等级。
第一等级为帮主，限一人；第二等级为副帮主，限一人；
第三等级为堂主，限三人；第四等级为帮众，数量由帮派实力和名望所限制。
以上“帮主”，“副帮主”，“堂主”，“帮众”的称谓可以由玩家自行设置。

等级有：0～4，0表示开除。
grant [你自己的id] 0 表示你自己主动脱离帮派，没有任何损失。
帮主可以开除帮派里的其他人，被帮主开除的人将受到3%的经验惩罚。

接收一个玩家加入帮派，此玩家必须先用“gjoin”命令发出请求。

HELP
	);
	return 1;
}

int main(object me, string arg)
{
	string uid;
	int gclass;
	object ob;

	if( !arg || sscanf(arg,"%s %d",uid,gclass)<2 || gclass<0 || gclass>4 )
		return help(me);
	if( !me->query("group/id") )
		return notify_fail("你并没有加入帮会呢。\n");
	if( !GROUP_D->site_now(me->query("group/id")) )
		return notify_fail( GROUP_D->get_last_error() );

	if( uid==me->query("id") && gclass==0 ) {
		if( me->query("group/class") == 1 )
			return notify_fail("解散帮会请用 gdismiss 命令。\n");
		if( !GROUP_D->set_group_master(me->query("group/id"),uid,gclass) )
			return notify_fail( GROUP_D->get_last_error() );
		if( !GROUP_D->set_group_member(me->query("group/id"),uid,0) )
			return notify_fail( GROUP_D->get_last_error() );
		GROUP_D->add_group_members(me->query("group/id"),-1);
		message_vision("$N退出了"+GROUP_D->get_group_name(me->query("group/id"))+"。\n",me);
		log_file("group/manage",sprintf("rebel   :%s:%s(%s)",me->query("group/id"),me->name(),me->query("id")),me);
		me->delete("group");
		me->delete_temp("title");
		return 1;
	}

	if( me->query("group/class") > 3 )
		return notify_fail("你的等级不够。\n");

	ob = find_player(uid);
	if (ob && !me->visible(ob)) ob = 0;

	if( gclass == 0 ) {
		if( !ob ) return notify_fail("没有这个人。\n");
		if( ob->query("group/id") != me->query("group/id") )
			return notify_fail("这个人并不是你们帮会的成员。\n");
		if( me->query("group/class") > 2 )
			return notify_fail("你的等级不够。\n");
		if( me->query("group/class") >= ob->query("group/class") )
			return notify_fail("你的等级不够，无法开除"+ob->name()+"。\n");

		if( !GROUP_D->set_group_master(me->query("group/id"),uid,gclass) )
			return notify_fail( GROUP_D->get_last_error() );
		if( !GROUP_D->set_group_member(me->query("group/id"),uid,0) )
			return notify_fail( GROUP_D->get_last_error() );
		GROUP_D->add_group_members(me->query("group/id"),-1);
		ob->add("combat_exp",-ob->query("combat_exp")*3/100);
		ob->delete("group");

		tell_room(environment(me),me->name()+"把"+ob->name()+"开除出"+GROUP_D->get_group_name(me->query("group/id"))+"。\n",({me,ob}));
		tell_object(me,"你把"+ob->name()+"开除出"+GROUP_D->get_group_name(me->query("group/id"))+"。\n");
		tell_object(ob,"你被"+me->name()+"开除出"+GROUP_D->get_group_name(me->query("group/id"))+"。\n");
		log_file("group/manage",sprintf("fire    :%s:%s(%s):%s(%s)",me->query("group/id"),me->name(),me->query("id"),ob->name(),ob->query("id")),me);
		ob->delete_temp("title");
	}
	else {
		if( !ob || !present(ob,environment(me)) ) return notify_fail("没有这个人。\n");
		if( ob->query("group") && ob->query("group/id")!=me->query("group/id") )
			return notify_fail("这个人并不是你们帮会的成员。\n");
		if( me->query("group/class")>=gclass || (ob->query("group/class") && me->query("group/class")>=ob->query("group/class")) )
			return notify_fail("你的等级不够，无法改变"+ob->name()+"在帮会中的等级。\n");
		if( !ob->query("group") && GROUP_D->get_group_members(me->query("group/id"))>=GROUP_D->get_max_group_members(me->query("group/id")) )
			return notify_fail("你们帮派的人数已经达到上限："+CHINESE_D->chinese_number(GROUP_D->get_max_group_members(me->query("group/id")))+"人。\n");
		if( !ob->query("group") && ob->query_temp("group/join")!=me->query("group/id") )
			return notify_fail(ob->name()+"并没有要求加入你们的帮会啊。\n");

		if( !ob->query("group") && !GROUP_D->set_group_member(me->query("group/id"),uid,1,ob->name()) )
			return notify_fail( GROUP_D->get_last_error() );
		if( !GROUP_D->set_group_master(me->query("group/id"),uid,0) )
			return notify_fail( GROUP_D->get_last_error() );
		if( !GROUP_D->set_group_master(me->query("group/id"),uid,gclass) )
			return notify_fail( GROUP_D->get_last_error() );
		if( !ob->query("group") ) {
			GROUP_D->add_group_members(me->query("group/id"),1);
			log_file("group/manage",sprintf("join    :%s:%s(%s):%s(%s)",me->query("group/id"),me->name(),me->query("id"),ob->name(),ob->query("id")),me);
		}
		ob->delete_temp("group/join");
		ob->set("group/id",me->query("group/id"));
		ob->set("group/name",GROUP_D->get_group_name(me->query("group/id")));
		ob->set("group/class",gclass);

		tell_room(environment(me),me->name()+"把"+ob->name()+"在帮派中的等级改为"+GROUP_D->get_member_class(me->query("group/id"),gclass)+"。\n",({me,ob}));
		tell_object(me,"你把"+ob->name()+"在帮会中的权限改为"+GROUP_D->get_member_class(me->query("group/id"),gclass)+"。\n");
		tell_object(ob,me->name()+"把你在帮派中的权限改为"+GROUP_D->get_member_class(me->query("group/id"),gclass)+"。\n");
		if(ob) GROUP_D->check_user_group(ob);
	}

	return 1;
}
