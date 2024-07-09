// Created by snowman@SJ 01/05/1999.
// Modified by snowman@SJ 26/08/2000.
// Modified by Looklove@SJ 28/08/2000
// Modified by Looklove@SJ 05/07/2001 优化进程，为保安全，更改信箱操作将同时通知原始信箱。
// 更改信箱操作的log文件，改为/log/static/change_mailbox

#include <ansi.h>
#include <mudlib.h>
#include <login.h>;

inherit F_DBASE;

int help(object me);
static object mail_ob = 0, linkob = 0;
static string pass, msg, msg2;

void create()
{
        seteuid(getuid());
        set("channel_id", "密码精灵");
}

string seed = "1AaBb2CcDd3EeFf4GgHh5IiJj6KkLl7MmNn8OoPp9QqRr0SsTtUuVvWwXxYyZz";

string make_password()
{
        int i = 8, j;
        string id = "";

        while (i--) {
                j = random(62);
                id += seed[j..j];
        }
        return id;
}

string change_passwd(object ob)
{
        if( !ob ) return 0;
        pass = make_password();
        ob->set("password",crypt(pass,"$1$ShuJian"));
        if( ob->query("newpassword") )
                ob->set("newpassword", crypt(pass,"$1$ShuJian"));
        write(HIW"玩家"+ ob->query("name")+ "(" + ob->query("id") + ")的密码已经更改。\n"NOR);
        return pass;
}

int main(object me, string dest)
{
        int Dest;
        string id, mail, wiz_status;

        if( me != this_player(1) ) return 0;

        if( wiz_level(me) < 4 )
                return notify_fail("只有 (admin) 和 (assist) 才能这样做。\n");

        if( !dest )
                return help(me);

        Dest = 0;
        seteuid(ROOT_UID);

        if( sscanf(dest, "%s %s", id, mail) != 2 )
                id = dest;
	if ("/cmds/usr/blacklist"->is_black(id))
		return notify_fail(id + " 是黑名单中的人物，不能更改密码。\n");
        mail_ob = LOGIN_D->find_body(id);

        if( !mail_ob || !interactive(mail_ob) ){
                linkob = new(LOGIN_OB);
                linkob->set("id",id);
                if( !linkob->restore() ) {
                        destruct(linkob);
                        return notify_fail("没有这个玩家。\n");
                } else {
                        wiz_status = SECURITY_D->get_status(linkob);
                        if (wiz_status == "(admin)" && SECURITY_D->get_status(me) != "(admin)"){
                                destruct(linkob);
                                return notify_fail("你不可以更改 Admin 的密码。\n");
                        }
                        if (!stringp(mail)) mail = linkob->query("email");
                        if (!stringp(mail) || sscanf(mail, "%*s@%*s.%*s") != 3) {
                                destruct(linkob);
                                return notify_fail("电子邮件地址错误，不能更改密码。\n");
                        }
                }
        } else {
                wiz_status = SECURITY_D->get_status(mail_ob);
                if (wiz_status == "(admin)" && SECURITY_D->get_status(me) != "(admin)")
                        return notify_fail("你不可以更改 Admin 的密码。\n");
                linkob = mail_ob->query_temp("link_ob");
                if( !linkob ) return notify_fail("此人的Link_ob错误。无法进行修改。\n");
                if( !stringp(mail) ){
                        mail = linkob->query("email");
                        if (!stringp(mail) || sscanf(mail, "%*s@%*s.%*s") != 3)
                                return notify_fail("电子邮件地址错误，不能更改密码。\n");
                }
                // Kick this player out afer changed his/her passwd.
                Dest = 1;
        }
	change_passwd(linkob);

        msg =   ""+linkob->query("name")+"("+linkob->query("id")+")" + "您好：\n"+
                "欢迎光临"+CHINESE_MUD_NAME+"！\n"+
                "请关闭自动登录功能，使用下面提供的密码登录。\n"+
                "\n"+
                ""+pass+ "\n"+
                "\n"+
                "用此密码登录成功后，建议您用 passwd 指令更换您的密码。\n"+
                "详情请在进入游戏后后使用“help passwd”指令,获得相关信息。\n"+
                ctime(time());

        msg2 =   ""+linkob->query("name")+"("+linkob->query("id")+")" + "您好：\n"+
                "欢迎光临"+CHINESE_MUD_NAME+"！\n"+
                "应您要求，您的注册信箱已由 "+linkob->query("email")+" 更改为 "+mail+"。\n"+
                "请关闭自动登录功能，使用下面提供的密码登录。\n"+
                "\n"+
                ""+pass+ "\n"+
                "\n"+
                "用此密码登录成功后，建议您用 passwd 指令更换您的密码。\n"+
                "详情请在进入游戏后使用“help passwd”指令，获得相关信息。\n"+
                ctime(time());                

        //设置玩家的注册信息
        if ( linkob->query("email") != mail ){
                log_file("static/change_mailbox", sprintf("%s %s(%s)将%s(%s)的注册信箱由 %s 改为 %s\n", ctime(time()),me->name(1),
                	me->query("id"), linkob->query("name"), linkob->query("id"), linkob->query("email"), mail));
		write("系统正在发送通知信件到"+ linkob->query("name")+ "(" + linkob->query("id") + ")的原始信箱。\n");
                SMTP_D->send_mail(me,linkob->query("email"),""+CHINESE_MUD_NAME+"信箱修改通知",msg2);
     	}
        else {
                log_file("static/sendpasswd", sprintf("%s %s(%s)修改了%s(%s)的密码 send to %s\n", ctime(time()),me->name(1),
                 	me->query("id"), linkob->query("name"), linkob->query("id"), mail));
       	}

     	write("开始发送通知信件到"+ linkob->query("name")+ "(" + linkob->query("id") + ")当前信箱。\n");
     	SMTP_D->send_mail(me,mail,""+CHINESE_MUD_NAME+"注册信件",msg);

        linkob->set("email", mail);
        linkob->save();
        if( Dest ){
                mail_ob->save();
                tell_object(mail_ob, HIW"巫师修改了您的注册密码，请退出后重新登录。\n"NOR);
                write("你修改了此人的注册密码，将他退离游戏。\n");
                destruct(mail_ob);
        }
        destruct(linkob);
        return 1;
}

int help(object me)
{
        write(@HELP
	指令格式 : sendpasswd ID [新email地址]  -> 发到此新地址，并自动改变注册mail地址。
      	或 	: sendpasswd ID               -> 发到注册mail地址。

	例： 	sendpasswd snowman snowman@sjever.net
     		sendpasswd snowman

	这个指令可以修改玩家的密码。
	修改注册信箱的操作容易损害玩家利益，请确保调查无误后使用。
	备注：
	更改信箱的操作，将同时向原始注册信箱和更改后的信箱发送信件。

HELP
        );
        return 1;
}
