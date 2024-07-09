// recovers.c
// Created by Numa 19990906
// Modified by Numa 19990926
// Modified by Numa 19991120 修复了玩家如果在线时恢复不成功的bug
// modify by snowman@SJ 26/02/2000 改了log file的位置
// modify by looklove 29/07/2000 修改部分返回信息
// modify by looklove 2001-7-5 发送通知信件 & 防止恢复已删除ID的s -f数据

#include <ansi.h>
#include <mudlib.h>

inherit F_CLEAN_UP;

private int update_player(object me);
int help(object me);

void create()
{
	seteuid(getuid());
}

int main(object me,string arg)
{
	object ob;
	string nod, msg;
	int *file_info;

	if (!arg) return help(me);
	me = this_player(1);
	if (sscanf(arg,"%s %s", arg, nod) != 2) {
		if (file_size("/data/user/"+ arg[0..0] + "/" + arg + ".ppp")>0) //add by looklove, 避免恢复被删除的ID 。
			return notify_fail(arg + " 此id是被删除的id，请加 -p 参数！\n");
		if (file_size("/data/login/" + arg[0..0] + "/" + arg + ".ooo") < 0)
			return notify_fail(arg + " 的登录档案备份不存在，无法恢复！\n");
		if (file_size("/data/user/"+ arg[0..0] + "/" + arg + ".ooo")<0)
			return notify_fail(arg + " 的数据档案备份不存在，无法恢复！\n");

		rename("/data/login/" + arg[0..0] + "/" + arg + ".ooo", "/data/login/" + arg[0..0] + "/" + arg + ".o");

		file_info = stat("/data/user/"+ arg[0..0] + "/" + arg + ".ooo");
		rename("/data/user/" + arg[0..0] + "/" + arg + ".ooo", "/data/user/" + arg[0..0] + "/" + arg + ".o");
		//log to recover_suicide
		log_file("static/recover_suicide",
			sprintf("%s %s恢复(%s)[自杀于%s]。\n",ctime(time()),geteuid(me),arg,ctime(file_info[1])));
		write(arg + " 的自杀数据恢复完毕，通知信件发送中……\n");

		//发送通知信件
		msg =   ""+arg+ "，您好：\n"+
			"    很高兴通知您，您在"+CHINESE_MUD_NAME+"的自杀ID已经恢复。欢迎您回来继续进行游戏。\n"+
			"如果您有任何意见和建议，请和所在分站的巫师联系。用“wizlist”指令可以\n"+
			"查看他们的ID。\n"+
			"\n"+
			"\n"+
			"此致\n"+
			"				书剑巫师组\n"+
			"\n"+ctime(time());

		SMTP_D->send_mail(me,arg,"通知邮件",msg);
	} else {
		if (nod != "-p") return help(me);
		if (geteuid(me) != "yuj" && geteuid(me) != "bbb")
			return notify_fail("请通知 bbb。\n");
		if (file_size("/data/login/" + arg[0..0] + "/" + arg + ".ppp") < 0)
			return notify_fail(arg + " 的登录档案备份不存在，无法恢复！\n");
		if (file_size("/data/user/"+ arg[0..0] + "/" + arg + ".ppp")<0)
			return notify_fail(arg + " 的数据档案备份不存在，无法恢复！\n");

		rename("/data/login/" + arg[0..0] + "/" + arg + ".ppp","/data/login/" + arg[0..0] + "/" + arg + ".o");

		file_info = stat("/data/user/"+ arg[0..0] + "/" + arg + ".ppp");
		rename("/data/user/" + arg[0..0] + "/" + arg + ".ppp","/data/user/" + arg[0..0] + "/" + arg + ".o");
		//log to recover_purge
		log_file("static/recover_purge",
			sprintf("%s %s恢复(%s)[删除于%s]。\n",ctime(time()),geteuid(me),arg,ctime(file_info[1])));
		write(arg + " 的删除数据恢复完毕，系统正在发送通知信件。\n");

		//发送通知信件
		msg =   ""+arg+ "，您好：\n"+
			"    您在"+CHINESE_MUD_NAME+"被删除的ID已经恢复。欢迎您回来继续进行游戏。\n"+
			"如果您有任何意见和建议，请和所在分站的巫师联系。用“wizlist”指令可以\n"+
			"查看他们的ID。\n"+
			"    请用“help rules”指令查看游戏规则，并请注意遵守。谢谢。\n"+
			"\n"+
			"\n"+
			"此致\n"+
			"				书剑巫师组\n"+
			"\n"+ctime(time());

		SMTP_D->send_mail(me,arg,"通知邮件",msg);
	}
	if (ob = LOGIN_D->find_body(arg))
		update_player(ob);
	return 1;
}

private int update_player(object me)
{
	object env, link_ob, obj;

	// First, create the new body.
	link_ob = me->query_temp("link_ob");
	if (!link_ob) {
		destruct(me);
		return 0;
	}
	link_ob->restore();
	obj = LOGIN_D->make_body(link_ob);
	if (!obj) return 0;

	// Exec the player to his/her link object.
	exec(link_ob, me);
	env = environment(me);

	destruct(me);

	// Restore new body and exec to it via enter_world in LOGIN_D
	obj->restore();
	LOGIN_D->enter_world(link_ob, obj, 1);

	tell_object(obj, "你的数据恢复完毕。\n");
	obj->write_prompt();
	return 1;
}

int help(object me)
{
write(@HELP
指令格式 : recovers <目标ID> [-p]

这个指令可以自动恢复自杀的玩家。
加上 -p 参数对 purge 进行恢复。

HELP
);
	return 1;
}
