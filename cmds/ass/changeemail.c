
#include <ansi.h>
#define SYNTAX  "指令格式：changemail <某人> <新信箱>\n"
inherit F_CLEAN_UP;


int main(object me, string str)
{
	string name, mail,oldmail;
	mixed ret;
 	       
	if(!str || sscanf(str, "%s %s", name, mail)!=2 )
		  return notify_fail(SYNTAX);
        
	ret = dbquery("SELECT U_Email FROM Users WHERE U_Username=\"" + name + "\"");

	if (!ret)
	{
		write("数据库连接失败。\n");
		return 1;
	}

	if (sizeof(ret) < 1) {
		write(HIY "没有这个玩家。\n"NOR);
		return 1;
	}
	else
	{
	        oldmail = ret[0][0];
	        
		ret = dbquery ("UPDATE Users set U_Email = \"" + mail + "\" where U_Username = \"" + name+ "\"");
		if (!ret)
		{
			write("更新玩家数据库失败。\n");
			return 1;
		}
		else
		{
			write("玩家 " + name + " 的信箱已经由 " + oldmail + " 更改为 " + mail + " 。\n");
			log_file("static/CHANGEEMAIL",
			sprintf("%s  被 %s(%s) 修改信箱由 %s 变成 %s 。\n",
				name, (string)me->query("name"),me->query("id"), oldmail,mail
			), ({ me }) );
		}
	}

	return 1;
}



int help(object me)
{
	write(@HELP
指令格式：changemail <某人> <新信箱>
你可以用这个指令给玩家换信箱。
HELP
	);
	return 1;
}
