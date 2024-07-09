// reply.c
// modified by snowman@SJ 25/11/2000

#include <ansi.h>
#include <net/dns.h>

inherit F_CLEAN_UP;

int help(object me);

int main(object me, string arg)
{
	object obj;
	string target, mud;

	if( !arg || arg=="" )
		return notify_fail("你要回答什么？\n");

	if (stringp(target = me->query_temp("reply"))) {
		if( sscanf(target, "%s@%s", target, mud)==2 ) {
			if (me->query("combat_exp") < 500000)
				return notify_fail("你的资历还不够，不能使用千里传音。\n");
			GTELL->send_gtell(lower_case(mud), lower_case(target), me, arg);
			write("网路讯息已送出，可能要稍候才能得到回应。\n");
			return 1;
		}
	}

	if (!objectp(obj = me->query_temp("reply")))
		return notify_fail("刚才没有人和你说过话。\n");

	if (!interactive(obj))
		return notify_fail("刚才和你说话的人断线了。\n");

	if (!wizardp(me) && me->query("id") == obj->query("env/block"))
		return notify_fail(obj->query("name")+"不想听你瞎掰！\n");
		
	write(GRN "你回答" + (me->visible(obj)?obj->query("name"):obj->name()) + "：" + arg + "\n" NOR);
	message("channel", sprintf(GRN"%s回答你：%s\n"NOR,
		obj->visible(me)?(me->query("name")+"("+capitalize(me->query("id"))+")"):me->name(), arg), obj);

	obj->set_temp("reply", me);
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式：reply <讯息>

你可以用这个指令和刚才用 tell 和你说话的使用者说话。

see also : tell
HELP
	);
	return 1;
}
