// recovers.c
// 恢复自杀玩家

#include <ansi.h>

string do_auto(object me, string arg)
{
        object ob;
        int *file_info;

        seteuid(ROOT_UID);
	if (file_size("/data/login/" + arg[0..0] + "/" + arg + ".ooo") < 0)
		return HIR + arg + " 的登录备份档案已删除，无法恢复。\n"NOR;
        if (file_size("/data/user/"+ arg[0..0] + "/" + arg + ".ooo")<0)
                return arg + " 的档案好象没有备份呦！\n";

	ob = find_player(arg);
	if (ob) {
		tell_object(ob, BLINK HIR"\n\n\n\t现在正在给您恢复档案中，请您半分钟后登录。\n\n"NOR);
		"/cmds/usr/quit"->main(ob);
	}

	if (cp("/data/login/" + arg[0..0] + "/" + arg + ".ooo", "/data/login/" + arg[0..0] + "/" + arg + ".o") <=0)
		return "登陆文件复制失败！\n";

	file_info = stat("/data/user/"+ arg[0..0] + "/" + arg + ".ooo");
	if (cp("/data/user/" + arg[0..0] + "/" + arg + ".ooo","/data/user/" + arg[0..0] + "/" + arg + ".o") <=0)
		return "数据文件复制失败！\n";
	write("数据已经恢复完毕，现在"HIW"记录"NOR"进程！\n");
//log to recover_suicide
	log_file("static/recover_suicide",sprintf("%s %s恢复(%s)[自杀于%s]。\n",ctime(time()),geteuid(me),arg,ctime(file_info[1])));
        write(arg + " 的自杀数据恢复完毕！\n");
        return "恢复玩家 "+ arg +" 成功。\n";
}

string *help()
{
	return ({"recovers [id]","可以自动恢复自杀的玩家。"});
}
