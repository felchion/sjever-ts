// snedpasswd.c
// 发送密码

#include <ansi.h>
#include <net/dns.h>
#include <net/telnet.h>
#include <net/socket.h>
#include <net/socket_errors.h>
#include <socket_err.h>
#include <mudlib.h>
#include <login.h>;

inherit F_DBASE;

static int my_socket,socket1,socket2;
static object me = 0, mail_ob = 0, linkob = 0;
static int status=0, connected = 0, Dest = 0;
static string smtp_ip="202.97.175.44 25";
static string smtp_domain="mymud.com";
static string sender="password@sjever.net";
static string rcpt,msg;

void create()
{
        seteuid(ROOT_UID);
        set("channel_id", "密码精灵");
}

void write_message(string);

int do_telnet(string arg)
{
        int err;

        if( connected ) socket_close(my_socket);
        my_socket = socket_create( STREAM, "in_read_callback","in_close_callback" );
        if ( my_socket < 0 ) {
//                tell_object(me, "创建套接字失败。\n" );
                return 0;
        }
        err = socket_connect( my_socket, arg , "read_callback","write_callback" );
        if( err==EESUCCESS ) {
                write(HIW"\n现在发送信件...\n"NOR);
                connected = 1;
                return 1;
        }
        write("邮件服务器无法连接。\n");
        return 0;
}

void close_socket()
{
        if( connected )  {
                if( socket_close(my_socket) ) connected = 0;
                my_socket = 0;
        }
}

void read_callback(int fd, mixed message)
{
        if (me && wizardp(me)) write(">> "+message);
        switch(status) {
        case 0:
                if(sscanf(message,"220%*s")) {
                        status++;
                        return;
                }
                break;
        case 1:
                if(sscanf(message,"250%*s")) {
                        status++;
                        write_message("MAIL FROM: <"+sender+">");
                        return;
                }
                break;
        case 2:
                if(rcpt&&sscanf(message,"250%*s")) {
                        status++;
                        write_message("RCPT TO: <"+rcpt+"> NOTIFY=NEVER");
                        return;
                }
                break;
        case 3:
                if(sscanf(message,"250%*s")) {
                        status++;
                        write_message("DATA");
                        return;
                }
                write("邮件地址错误，请检查！\n");
                break;
        case 4:
                if(sscanf(message,"354%*s")) {
                        status++;
                        write_message(msg+"\n.\n");
                        return;
                }
                break;
        case 5:
                if(sscanf(message,"451%*s")) write("发信出错！请稍候再试。\n");
                else {
                        write("注册信件已经正确发送了。\n");
                        if( linkob ) destruct(linkob);
                        mail_ob = 0;
                }
                
                break;                          
        }
        if(status!=5 || !sscanf(message,"250%*s")) write("发信出错！\n");
        status=0;
        write_message("QUIT");
        close_socket();
	if (linkob) destruct(linkob);
}

void write_callback(int fd)
{
        write_message("HELO " +smtp_domain );
}

void write_message(string arg)
{
        if( !arg ) arg="";
        if (me && wizardp(me)) write("<< "+arg+"\n");
        socket_write(my_socket, arg+"\n");
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

private string change_passwd(object ob)
{
        string pass;
        
        if( !ob ) return 0;
        pass = make_password();        
        ob->set("password",crypt(pass,"$1$ShuJian"));
        if( ob->query("newpassword") )
                ob->set("newpassword", crypt(pass,"$1$ShuJian"));
        write("玩家"+ ob->query("name")+ "(" + ob->query("id") + ")的密码已经更改，发送信件成功。\n");
        return pass;
}

private string do_auto(object me, string dest)
{
        string id, mail, wiz_status;
        
        if( mail_ob ) return "正在办理他人的密码手续，请稍候。\n";
        
        if( me != this_player(1) ) return "身份验证失败！\n";
        
        if (!do_telnet(smtp_ip)) {
                mail_ob = 0;
                return "do_telnet -> Error.\n";
        }
        Dest = 0;
        seteuid(ROOT_UID);
        
        id = dest;
        mail_ob = LOGIN_D->find_body(id);
        
        if( !mail_ob || !interactive(mail_ob) ){
                linkob = new(LOGIN_OB);
                linkob->set("id",id);
                if( !linkob->restore() ) {
                        mail_ob = 0;
                        destruct(linkob);
                        return "没有这个玩家。\n";
                } 
                else {         
                        wiz_status = SECURITY_D->get_status(linkob);
                        if( wiz_status == "(admin)" && SECURITY_D->get_status(me) != "(admin)" ){
                                mail_ob = 0;
                                destruct(linkob);
                                return "你不可以更改 Admin 的密码。\n";
                        }
                }
        }
        
        else{
                wiz_status = SECURITY_D->get_status(mail_ob);
                if( wiz_status == "(admin)" && SECURITY_D->get_status(me) != "(admin)" ){
                        mail_ob = 0;
                        return "你不可以更改 Admin 的密码。\n";
                }
                  
                linkob = mail_ob->query_temp("link_ob");
                if( !linkob ) {
                        mail_ob = 0;
                        return "此人的Link_ob错误。无法进行修改。\n";
                }
                
                if( !stringp(mail) ){
                        mail = linkob->query("email");
                        if( !stringp(mail) || sscanf(mail, "%*s@%*s.%*s") != 3 )
                                return "此人的电子邮件地址错误，不能更改密码。\n";
                }
                // Kick this player out afer changed his/her passwd.
                Dest = 1;
        }

        if( !stringp(mail) ){
                mail = linkob->query("email");
                if( !stringp(mail) || sscanf(mail, "%*s@%*s.%*s") != 3 ){
                        mail_ob = 0;
			destruct(linkob);
                        return "此人的电子邮件地址错误，不能更改密码。\n";
                }
        }
        rcpt = mail;
        status = 0;
        write("开始办理密码注册手续及发送至 "+ mail +" ...\n");
        //设置玩家的注册信息
        if ( linkob->query("email") != mail )
        	log_file("static/sendpasswd", sprintf("%s %s(%s)修改了%s(%s)的密码 send to %s\n", ctime(time()),me->name(1),
                 me->query("id"), linkob->query("name"), linkob->query("id"), mail));

        msg=
                "SUBJECT: "+CHINESE_MUD_NAME+"注册信件！\n"+
                "FROM: \""+"密码管理员@"+CHINESE_MUD_NAME+"\" <password@sjever.net>\n"+
                "TO: \""+linkob->query("name")+"("+linkob->query("id")+")"+"\" <"+dest+">\n"+
                "MIME-Version: 1.0\n"+
                "DATE: "+ctime(time())+"\n"+
                "X-MAILER: LPC SMTP Client v0.4\n\n\n"+
                linkob->query("name")+"("+linkob->query("id")+")" + " 您好！欢迎光临"+CHINESE_MUD_NAME+"！\n"+
                "请关闭自动登录功能，使用下面提供的密码登录。\n\n"+
                "你的注册密码是："+ change_passwd(linkob) +"\n\n"+
                "用此密码登录成功后，请用 passwd 指令更换您的密码。\n"+
                  "详情请参考 help passwd 。\n"+
                "总站主页：http://www.sjever.net\n"+
                ctime(time());

        linkob->set("email", mail);
        linkob->save();
        remove_call_out("fail");
        call_out("fail", 90, mail_ob);
        if( Dest ){
                mail_ob->save();
                tell_object(mail_ob, HIW"有人修改了您的注册密码，请退出后重新登录。\n"NOR);
                tell_object(me, "你修改了此人的注册密码，将他退离游戏。\n");
                destruct(linkob);
                catch(destruct(mail_ob));
                if( mail_ob ) 
                	return "你无法让这个玩家离线。\n";
                CHANNEL_D->do_channel(this_object(), "sys", "密码更动 -> "NOR + id + HIR" 退出了系统，密码发送至 "+mail+" 。");
        }
        else {
        	CHANNEL_D->do_channel(this_object(), "sys", "密码更动 -> "NOR + id + HIR" 的密码更新，发送至 "+mail+" 。");
	}
        return "发送玩家 "+ dest +" 密码成功，密码信件已经发往 "+ mail +"。\n";
}

void fail(object ob)
{
        if (!ob || ob != mail_ob) return;
        close_socket();
        message_vision("密码注册失败！\n", me);
        write("请重新注册。\n");
        mail_ob = 0;
}


string *help()
{
	return ({"passwd [id]","发送新密码到玩家注册mail地址。"});
}
