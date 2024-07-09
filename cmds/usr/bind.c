#include <ansi.h>

private void confirm_passwd(string pass, object me, object ob);
int main(object me, string arg)
{
	object ob;
	mixed list;
	int i;
	if (!me) return 0;
	if (!arg) {
		if ( mapp(me->query("Bind_ID")) ) {
			list = keys(me->query("Bind_ID"));
			for(i=0;i<sizeof(list);i++)
				write(sprintf("%s ",capitalize(list[i])));
			return notify_fail("\n现在有以上帐号和你的帐号绑定在一起!\n");
		}
		else if ( me->query("ID_Bind") ) 
			return notify_fail("你的帐号现在和"+me->query("ID_Bind")+"绑定在一起!\n");
		else
			return notify_fail("你的帐号没有和其它ID绑定!\n");
	}
	
	ob = find_player(arg);
	if (!ob) 
		return notify_fail("这个用户不在线，要绑定请先登录！\n");
	
	if (!userp(ob)) 
		return notify_fail(ob->query("ID")+"是什么东东！\n");		
	
	if (ob==me) 
		return notify_fail("不能和自己绑定！\n");
	
	if ( mapp(me->query("Bind_ID")) ) 
		return notify_fail("已经有其它帐号和这个帐号绑定在一起！\n");
			
	if ( ob->query("ID_Bind") && ob->query("ID_Bind")!=ob->query("id") )
		return notify_fail(ob->query("id")+"已经和其它帐号绑定了！\n");
	if ( me->query("ID_Bind") )
		return notify_fail("你的帐号已经和"+me->query("ID_Bind")+"绑定了！\n");		

	write(sprintf(HIR"请输入%s(%s)的登录密码："NOR,ob->query("name"),ob->query("id")));
	input_to("confirm_passwd", me,ob);
	return 1;
}

private void confirm_passwd(string pass, object me, object ob)
{
	string confirm_pass;
	confirm_pass = PASSWD_D->get_passwd(ob->query("id"));

	if ( confirm_pass && crypt(pass, confirm_pass) != confirm_pass) {
		write("密码错误！\n");
		return;
	}
  me->set("ID_Bind",ob->query("id"));
  ob->set("ID_Bind",ob->query("id"));
  ob->set("Bind_ID/"+me->query("id"),1);
  write(HIY"帐号绑定成功！\n"NOR);
	return;
}

int help(object me)
{	write("\n指令格式 : bind <ID>\n");
	write(@HELP

这个指令可以让你把帐号绑定到另一个帐号上和查看帐号的绑定情况。
注意：帐号一旦绑定将无法解除，请大家谨慎使用！

HELP);

    return 1;
}
