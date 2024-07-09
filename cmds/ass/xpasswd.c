//cpasswd.c by XiaoYao
// Modify by snowman@SJ 01/05/1999.

#include <ansi.h>
#include <login.h>;

string seed = "1AaBb2CcDd3EeFf4GgHh5IiJj6KkLl7MmNn8OoPp9QqRr0SsTtUuVvWwXxYyZz";

object get_player(string arg);

string make_password()
{
	int i = 8, j;
	string id = "";

	while (i--)
	{
		j = random(62);
		id += seed[j..j];
	}

	return id;
}

string change_passwd(object ob)
{
	string new_pass;

	if( !ob )
	{
		return 0;
	}

	new_pass = make_password();

	if ( !ob->set("confirm_passwd", crypt(new_pass, "$1$ShuJian")))
	{
		return 0;
	}

	write(HIW"\n玩家" + ob->query("name") 
		+ "(" + ob->query("id") + ")的确认密码已经更改。\n"NOR);
	
	return new_pass;
}

int main(object me, string arg)
{
	object ob;
	string id,password;
	string new_pass;
	
	seteuid(ROOT_UID); // add by Yuj@SJ 1999-6-2
	if( me != this_player(1) ) return 0;
	
	if( wiz_level(me) < 4 )
		return notify_fail("只有 admin 和 assist 才能这样做。\n");
		
	if(!arg||sscanf(arg,"%s",id,password)!=1)
		
				return notify_fail(" xpasswd ID\n");


	ob = LOGIN_D->find_body(id);
	if(!ob){
		if(!objectp(ob = get_player(id))) 
		{
			return notify_fail("没有这个人吧?\n");
		} else {				    
			
			if (!(new_pass = change_passwd(ob)))
			{
				return notify_fail("取密码发生错误，请通知开发人员，暂时无法进行修改。\n");
			}
			write("玩家" + ob->query("id") + "("+ob->query("name")+ ")的密码已经更改为："+new_pass+"。\n");
			log_file( "changepw.log", sprintf("%s %s(%s)修改了%s(%s)的确认密码\n", ctime(time()), me->query("name"), me->query("id"),
				 ob->query("name"), ob->query("id")));
				 
			ob->set_temp("db_quit", 1);
			ob->save();
			destruct(ob);
			return 1;
			}
	}
	
			if (!(new_pass = change_passwd(ob)))
			{
				return notify_fail("取密码发生错误，请通知开发人员，暂时无法进行修改。\n");
			}
			write("玩家" + ob->query("id") + "("+ob->query("name")+ ")的密码已经更改为："+new_pass+"。\n");
			log_file( "changepw.log", sprintf("%s %s(%s)修改了%s(%s)的确认密码\n", ctime(time()), me->query("name"), me->query("id"),
				 ob->query("name"), ob->query("id")));
				 
		return 1;

}

object get_player(string arg)
{
	object user;	
	
	user = new(USER_OB);
	if (!user) {
		write("现在可能有人正在修改使用者物件的程序，无法进行复制。\n\n");
		return 0;
	}
	seteuid(arg);
	export_uid(user);
	seteuid(getuid());
	if(!user->restore()) {
	destruct(user);
	return 0;}
	user->set_name(user->query("name"), ({ arg}));
	return user;
}

int help(object me)
{
	write(@HELP
指令格式 : xpasswd ID

这个指令可以修改玩家的确认密码，请谨慎使用。

HELP
    );
    return 1;
}
