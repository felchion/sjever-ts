// restore.c
// modify by looklove 2001.7.4

inherit F_CLEAN_UP;

#include <ansi.h>
#include <mudlib.h>

private int do_purge_players(int day);
int help(object me);

int main(object me, string arg)
{

	if (me!=this_player(1) || wiz_level(me) < wiz_level("(arch)")||wiz_level(me) < wiz_level(arg))
		return notify_fail("你没有权力使用这个指令。\n");

	if( !arg ) return help(me);
	
	if (!dbquery("REPLACE into Data select * FROM Backup WHERE U_ID='" 
                        + DATA_DIR + "user/" + arg[0..0] + "/" + arg + ".o"+"' order by U_Time DESC LIMIT 1"))
			return notify_fail("用户备份数据读取失败!\n");
	
	if (!dbquery("REPLACE into Data select * FROM Backup WHERE U_ID='" 
                        + DATA_DIR + "login/" + arg[0..0] + "/" + arg + ".o"+"' order by U_Time DESC LIMIT 1"))
			return notify_fail("用户备份登录数据读取失败!\n");			
	
	if (!dbquery("replace into Users select * from Users_backup where U_Username='" + arg + "'"))
			return notify_fail("用户登录数据恢复失败!\n");
	else
			dbquery("delete from Users_backup where U_Username='" + arg + "'");
	
	if (!dbquery("replace into members select * FROM BBSID_backup WHERE username = '"+arg +"@"+lower_case(INTERMUD_MUD_NAME)+"'"))
			return notify_fail("用户论坛数据恢复失败!\n");
	else
			dbquery("delete FROM BBSID_backup where username = '"+arg +"@"+lower_case(INTERMUD_MUD_NAME)+"'");
			
	tell_object(me,HIY"用户数据恢复成功！\n"NOR);
	
	return 1;
	
}

int help(object me)
{
write(@HELP
指令格式：restore <使用者ID>

恢复玩家最近的备份数据。
HELP
);
    return 1;
}

