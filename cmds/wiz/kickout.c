// kickout.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	string name,q_name,q_id,q_ip;
	object obj, link_ob;

	if( !arg ) return notify_fail("指令格式 : kickout <玩家名>\n" );

	seteuid(ROOT_UID);

	obj = LOGIN_D->find_body(arg);
	if( !obj || !userp(obj) || !me->visible(obj)) return notify_fail("没有这个玩家....。\n");
	name = obj->short(1);
	q_name = obj->name(1);
	q_id = obj->query("id");
	q_ip = query_ip_name(obj);

	if( me == obj )
		return notify_fail("用 quit 这条指令好些 :) \n");

	if (wiz_level(me) <= wiz_level(obj))
		return notify_fail("想对"+q_name+"下手？很遗憾。。。你不能这么做。\n");

	message_vision("一个黑影突然出现在$N面前，说道：“"+q_name +
			"！你因为违反游戏规则，请退出游戏吧！”\n", obj);
	obj->dismiss_team();
	obj->save();
	link_ob = obj->query_temp("link_ob");
	if (link_ob) {
		link_ob->save();
		destruct(link_ob);
	}
	catch(destruct(obj));
	
	if( obj ) return notify_fail("你无法让这个玩家离线。\n");
	else {
		//set("channel_id", me->name() );
		CHANNEL_D->do_channel(this_object(), "rumor", q_name + "因为违反游戏规则，被系统踢出了"+MUD_NAME+"。");
		CHANNEL_D->do_channel(me, "wiz", "我将"NOR + name + HIR"踢出了系统。");
		log_file("static/kickout",sprintf("%s(%s) kickout %s(%s) on %s at %s.\n", me->name(), 
			me->query("id"), q_name, q_id, q_ip, ctime(time()) ) );
		write("哈哈哈哈哈哈... 好爽!\n");
	}
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式 : kickout <玩家名>

此命令可以强迫恶意的玩家离线。

HELP );
	return 1;
}
