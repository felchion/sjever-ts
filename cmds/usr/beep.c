// beep.c
// by snowman 11/03/2000

inherit F_CLEAN_UP;

#include <ansi.h>

int help(object me);

void remove_busy(object me)
{
	if ( me) me->delete_temp("command_busy");
}

int main(object me, string arg)
{
	string *friend_user;
	object obj;

	if( me->query_temp("command_busy"))
		return notify_fail("你现在正忙着呢。\n");

	if( !arg ) return help(me);

	if( arg == me->query("id") )
		return notify_fail("Beep自己？好主意耶。\n");

	friend_user = me->query("friend_user");
	if( !wizardp(me) && (!friend_user || !sizeof(friend_user)) )
		return notify_fail("你目前还没有设定好友，不可以用 beep。\n");

	if( !wizardp(me) && member_array(arg, friend_user) == -1)
		return notify_fail("好友名单中没有此人，不可以用 beep。\n");

	if (!obj = LOGIN_D->find_body(arg))
		return notify_fail(arg+"这个人现在不在线上。\n");

	if (!me->visible(obj))
		return notify_fail(arg+"这个人现在不在线上。\n");

	if (!interactive(obj))
		return notify_fail("这个人断线了。\n");
	if (!living(obj))
		return notify_fail("这个人现在无法听见你的话。\n");

	if( !wizardp(me) ){
		friend_user = obj->query("friend_user");
		if( !friend_user || !sizeof(friend_user) )
			return notify_fail("对方目前还没有设定好友，不可以用 beep。\n");

		if( !wizardp(me) && member_array(me->query("id"), friend_user) == -1)
			return notify_fail("对方的好友名单中没有你，不可以用 beep。\n");
	}

	tell_object(me, HIG "【呼叫】"+obj->query("name")+"…… Beep !\n"NOR);
	tell_object(obj, HIG "\n【呼叫】…… Beep …… 你的朋友"+me->query("name")+"("+me->query("id")+")呼叫你呢，快和"
	+gender_pronoun(me->query("gender"))+"联系。\n" NOR);

	me->set_temp("command_busy", 1);
	call_out("remove_busy", 3, me);
	return 1;

}

int help(object me)
{
write(@HELP

指令格式：beep <某人>

你可以用这个指令来呼叫自己的朋友(对robot特别有用)。
条件是双方都在对方的好友名单内。如果对方电脑(PC)接
上了小喇叭的(不是音响)，就能听见Beep声。

相关指令：finger ( 用于加入好友 )
HELP);
return 1;
}
