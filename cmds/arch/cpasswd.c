//cpasswd.c by XiaoYao
// Modify by snowman@SJ 01/05/1999.

#include <login.h>;

int main(object me, string arg)
{
	object ob,link_ob;
	string id,password;
	
	seteuid(ROOT_UID); // add by Yuj@SJ 1999-6-2
	if( me != this_player(1) ) return 0;
	
	if( wiz_level(me) < 4 )
		return notify_fail("只有 admin 和 assist 才能这样做。\n");
		
	if(!arg||sscanf(arg,"%s %s",id,password)!=2)
		return notify_fail("用法： cpasswd <ID> <新密码> \n此操作容易损害玩家利益，你必须先调查清楚！\n");

	ob = LOGIN_D->find_body(id);
	if(!ob){
		ob = new(LOGIN_OB);
		ob->set("id",id);
		if( !ob->restore() ) {
			destruct(ob);
			return notify_fail("没有这个玩家。\n");
		} else {				    
			ob->set("password",crypt(password, "$1$ShuJian"));
			if (!PASSWD_D->set_passwd(ob->query("id"), crypt(password, "$1$ShuJian"))) 
				write("数据库该玩家密码修改失败！\n");
			write("玩家" + ob->query("id") + "("+ob->query("name")+ ")的密码已经更改为："+password+"。\n");
			log_file( "changepw.log", sprintf("%s %s(%s)修改了%s(%s)的密码\n", ctime(time()), me->query("name"), me->query("id"),
				 ob->query("name"), ob->query("id")));
			ob->save();
			destruct(ob);
			return 1;
			}
	}
	
	link_ob=ob->query_temp("link_ob");
	if(link_ob) {
		link_ob->set("password",crypt(password,"$1$ShuJian"));
		link_ob->save();
		if (!PASSWD_D->set_passwd(link_ob->query("id"), crypt(password, "$1$ShuJian"))) 
				write("数据库该玩家密码修改失败！\n");
		write("玩家" + ob->query("id") + "("+ob->query("name") + ")的密码已经更改为："+password+"。\n");
		log_file( "changepw.log", sprintf("%s %s(%s)修改了%s(%s)的密码。\n", ctime(time()), me->query("name"), me->query("id"),
				 ob->query("name"), ob->query("id")));
		return 1;
	}
	else
		return notify_fail("错误。无法进行修改。\n");
	return 1;
}


int help(object me)
{
	write(@HELP
指令格式 : cpasswd ID 新密码

这个指令可以修改玩家的密码，请谨慎使用。

HELP
    );
    return 1;
}
