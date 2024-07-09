// shutdown.c with time loop.
// by snowman@SJ  22/02/1999
// Modify BY LInux for 鬼古术

#include <ansi.h>
#include <mudlib.h>
#include <net/daemons.h>

int counting = 0;
int min;

int help (object me)
{
	write(@HELP
指令格式: reboot < min|now >

例：
	reboot       :  显示目前的信息，如果没在reboot当中，显示本信息。
	reboot now   :  马上重新起动游戏。
	reboot 3     :  等三分钟，然后重新起动游戏。
	reboot cancel:  取消重起动。

HELP
	);
	return 1;
}

void create()
{
	seteuid(getuid());
}

private int start_shutdown()
{
	message("channel",
		HIW"\n\t"+CHINESE_MUD_NAME+HIW"将在 "HIR+chinese_number(min)+HIW" 分钟后重新启动\n"
		"\t为了避免不必要的档案出错，请在线玩家尽快自行退出。\n\n" NOR, users()
	);
	call_out("countdown", 60);
	counting = 1;
	return 1;
}

int main(object me, string arg)
{
	int time;

	if (geteuid(me) != ROOT_UID) {
	if (me != this_player(1))
		return 0;

	if (SECURITY_D->wiz_level("(arch)") > SECURITY_D->wiz_level(me))
		return notify_fail("只有 (arch) 以上的巫师才能重新启动"+CHINESE_MUD_NAME+"。\n");

	if (!arg || arg == "?") {
		if (!counting)
			return help(me);
		return notify_fail(CHINESE_MUD_NAME+HIW"正在倒计时中，离重新启动还有"HIR+ chinese_number(min) + HIW"分钟。\n"NOR);
	}

	if (arg == "cancel") {
		remove_call_out("countdown");
		remove_call_out("do_shutdown");
		counting = 0;
		LOGIN_D->set_wizlock(0);
		message("system","\n\t"+CHINESE_MUD_NAME+HIW"重新启动"HIR"取消"HIW"。\n"NOR, users());
		return notify_fail(CHINESE_MUD_NAME+HIW"重新启动取消。\n"NOR);
	}
	}
	log_file("static/CRASHES", geteuid(me) + " 重启动 "+CHINESE_MUD_NAME+" 在 "
		+ ctime(time()) + "\n", me);

	if (arg == "now") {
		LOGIN_D->set_wizlock(1);
		message("system","\n\t"+CHINESE_MUD_NAME+HIW"重新启动。请稍候再尝试连线。\n"NOR, users());
		call_out("do_shutdown", 1);
		return 1;
	}

	if (sscanf(arg, "%d", time) != 1
	|| time < 1)
		return help(me);

	if (counting)
		return notify_fail(CHINESE_MUD_NAME+HIW"正在倒计时中，离重新启动还有"HIR+ chinese_number(min) + HIW"分钟。\n"NOR);

	min = time;
	if (start_shutdown()) {
		LOGIN_D->set_wizlock(1);
		write("Starting shutdown...\n");
	} else
		write("定时Reboot有问题，请检查 /cmds/arch/reboot.c\n");
	return 1;
}

private void countdown()
{
	if (--min) {
		message("channel",
			HIW"\n\t"+CHINESE_MUD_NAME+HIW"将在 "HIR+chinese_number(min)+HIW" 分钟后重新启动\n"
			"\t为了避免不必要的档案出错，请在线玩家尽快自行退出。\n\n" NOR, users()
		);
		call_out("countdown", 60);
	} else {
		message("system",HIW"\n\t"+CHINESE_MUD_NAME+HIW"重新启动。请稍候再尝试连线。\n\n"NOR,users());
		call_out("do_shutdown", 1);
	}
}

private void do_shutdown()
{
	object *users, link_ob;
	int i;
	
	
	if (find_object(DNS_MASTER))
		DNS_MASTER->send_shutdown();

	users = filter_array(objects(), (: userp :));
	foreach (object user in users) {
		if (!environment(user))
			continue;
		reset_eval_cost();
		user->save();
		i = uptime() - user->query_temp("time");
		link_ob = user->query_temp("link_ob");
		if (objectp(link_ob)){
			//－－－增加保存			
			link_ob->set("last_on", time() + (i > 300 ? -30 : 0));
			link_ob->set("last_from", link_ob->query_temp("last_from"));
			link_ob->set_temp("db_quit", 1);
			//end
			link_ob->save();
			
		}
	}

	write_file("/log/static/LASTCRASH", CHINESE_MUD_NAME+"在 "
		+ ctime(time()) + " 重新启动\n", 1);

	shutdown(0);
}
