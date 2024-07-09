// getpasswd.c
//允许玩家通过自己设定的密码提示问题以及原注册邮箱地址设置新的密码

inherit F_CLEAN_UP;
inherit F_DBASE;

#include <ansi.h>
#include <mudlib.h>
#include <login.h>
private int mysql = 0;
int help(object);
int check_state(object);
private void send_mail(string, object);

string seed = "1AaBb2CcDd3EeFf4GgHh5IiJj6KkLl7MmNn8OoPp9QqRr0SsTtUuVvWwXxYyZz";
string pass;

static object target_ob = 0;
static object link_ob = 0;

void create()
{
        seteuid(getuid());
}

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



int main(object me, string arg)
{
        
        if( me != this_player(1) ) return 0;

        if (!arg || arg=="") 
                return help(me);

        //if(!wizardp(me) && me->query("id")!="guest")    
                
                
        if ("/cmds/usr/blacklist"->is_black(arg))
        {
                write(arg + " 是黑名单中的人物，不能恢复密码,请等待巫师处理结果。\n");
                return 1;
        }
        if (arg == me->query("id"))
        {
                write(arg + " 不就是你自己吗？\n");
                return 1;
        }
        if (me->query_temp("getpasswd/mail") > 3)
        {
                write("系统检测到您多次输入错误的邮箱地址，暂时禁止使用该命令。\n");
                return 1;
                
        }
        if (me->query_temp("getpasswd/answer") > 2)
        {
                write("系统检测到您多次输入错误的问题答案，暂时禁止使用该命令。\n");
                return 1;
        }
                
        write(HIG"
玩家自己发送遗忘帐号的密码回注册信箱，是指玩家一旦遗忘自己的某个帐号
在书剑游戏中的密码，并且确保该帐号在超过七天没有登陆，如果您可以正确
回答该帐号的注册信箱，那么系统会发送密码到该帐号的注册信箱。

如果该帐号已经设置问题保护，那么，您必须回答正确问题才可以发送密码。
\n\n"NOR);

        
        target_ob = LOGIN_D->find_body(arg);
        
        if(target_ob && !interactive(target_ob))
        	return notify_fail(target_ob->query("name") +"("+target_ob->query("id")+")目前正在游戏中，您想干什么？\n"); 
        
        if(!target_ob || !interactive(target_ob))
        {
                link_ob = new(LOGIN_OB);
                link_ob->set("id",arg);
                if( !link_ob->restore() ) 
                {
                        destruct(link_ob);
                        return notify_fail("没有这个玩家。\n");
                }
                if( wiz_level(link_ob->query("id")))
                {
                        write("葛哈葛哈，你想葛哈？！！\n");
                        destruct(link_ob);
                        return 1;                       
                }
                if (!wizardp(me) && !check_state(link_ob))
                {
                        write(link_ob->query("name") +"("+link_ob->query("id")+")在最近七天内登陆过游戏，您不能索取密码。\n");
                        destruct(link_ob);
                        return 1;
                }
                else
                {
                        write(link_ob->query("name") +"("+link_ob->query("id")+")的当前情况符合玩家自己找回密码条件！\n");
                        write("请输入"+link_ob->query("name") +"("+link_ob->query("id")+")的当前注册信箱：");
                        input_to("check_mail", 1, link_ob);                     
                }
                return 1;
        }
        
        if( wiz_level(target_ob->query("id")))
                return notify_fail("葛哈葛哈，你想葛哈？！！\n");
        else
                return notify_fail(target_ob->query("name") +"("+target_ob->query("id")+")目前正在游戏中，您想干什么？\n");
}

private void check_mail(string mail, object ob)
{
	object me = this_player();
	// Modified by mxzhao 2004/04/11
	mixed ret;
	//mixed rows,*res;
	// End
	
	string right_mail = ob->query("email");
	
	if (!stringp(mail) || sscanf(mail, "%*s@%*s.%*s") != 3)
	{
		write("\n很明显，您输入的邮件地址格式不对！\n");
		destruct(ob);
		return;
	}
	
	if (right_mail != mail)
	{
		write("\n对不起，您输入的邮箱地址不是"+ob->query("name")+"("+ob->query("id")+")的注册邮箱地址。\n");
		write("请注意，系统已经记录您的尝试行为。\n");
		me->add_temp("getpasswd/mail",1);
		log_file("static/GETPASSWD", sprintf("%s %s 试图尝试 %s 的注册信箱。 \n", ctime(time()),this_player()->query("id"),ob->query("id")));             
		destruct(ob);   
		return;
	}
	
	// Modified by mxzhao 2004/04/11
	ret = dbquery("select Question, Answer from Users "
		"where U_Username=\""+ob->query("id")+"\"");

	if (!ret)
	{
		write(HIY"数据库连接失败。\n"NOR);
		return;
	}
	
	if (sizeof(ret) == 0 
		|| ret[0][0] == "")
	{
		write("\n请稍等，系统准备发送密码至注册邮箱"+ mail +"。\n");    
		send_mail(mail ,ob);
		return;
	}

	if (sizeof(ret) != 1)
	{
		write("数据库数据有问题，请与管理人员联系。\n");
		return;
	}

	write(HIG"\n该帐号设置了问题和答案保护，"
		"您只有回答正确下面这个答案后才可以发信到注册信箱。\n");
	write(HIW"\n请回答："YEL + ret[0][0] + NOR);
	write(HIW"\n答  案："NOR);

	input_to("get_answer", ob, ret[0][1], mail);

	/*
	if ( !intp(mysql = db_connect("localhost","mud","root")) ) 
	{
		write("\n连接数据库失败。请稍候再试。\n");
		return;
	}
	rows = db_exec(mysql, "select Question, Answer from " + INTERMUD_MUD_NAME + "_Users where U_Username=\""+ob->query("id")+"\"");
	write ("select Question,Answer from " + INTERMUD_MUD_NAME + "Users where U_Username=\""+ob->query("id")+"\"");
	if (!rows){
		write("\n请稍等，系统准备发送密码至注册邮箱"+ mail +"。\n");
		send_mail(mail ,ob);
		return;	
	}
	if (stringp(rows)) 
	{
		write("\n查询数据库失败。\n");
		if (wizardp(ob)) write(HIR"数据库结构出现问题。\n"HIR);
		db_close(mysql);
		return;
	}
	res = db_fetch(mysql, 1);
	db_close(mysql);
	if( res[0] != "")
	{
		write(HIG"\n
			该帐号设置了问题和答案保护，您只有回答正确下面这个答案后才可以发信到注册信箱。\n");
			write(HIW"\n请回答："YEL+res[0]+NOR);
		write(HIW"\n答  案："NOR);
		input_to("get_answer", ob ,res [1], mail);
	}
	else
	{
		write("\n请稍等，系统准备发送密码至注册邮箱"+ mail +"。\n");    
		send_mail(mail ,ob);
		return;
	}
	*/
	// End
}

private void get_answer(string answer, object ob,string e_answer, string mail)
{
        if (answer != e_answer && !wizardp(this_player()) ) {
                write("\n对不起，您回答的答案错误！请仔细考虑后重新尝试！\n");
                write("请注意，系统已经记录您的尝试行为。\n");
                this_player()->add_temp("getpasswd/answer",1);
                log_file("static/GETPASSWD", sprintf("%s 试图尝试 %s 的问题答案。 \n", this_player()->query("id"),ob->query("id")));             
                destruct(ob);
                return;
        }
        write(HIG"\n答案正确！\n");
        write("请稍等，系统准备发送密码至注册邮箱"+mail+"。\n");    
        send_mail(mail ,ob);
        return;
}
private string change_passwd(object ob)
{
        if( !ob ) return 0;
        pass = make_password();
        ob->set("password",crypt(pass,"$1$ShuJian"));
        if( ob->query("newpassword") ){
                ob->set("newpassword", crypt(pass,"$1$ShuJian"));
        }
        if (PASSWD_D->set_passwd(ob->query("id"), crypt(pass, "$1$ShuJian"))) 
        	write(HIW"玩家"+ ob->query("name")+ "(" + ob->query("id") + ")的密码已经更改。\n"NOR);
        else
        	write(HIW"系统自动修改玩家"+ ob->query("name")+ "(" + ob->query("id") + ")的修改失败，请报告开发人员。"NOR);
        return pass;
}
private void send_mail(string mail, object ob)
{
        string msg;
        change_passwd(ob);
        ob->save();
        
        msg =   ""+ob->query("name")+"("+ob->query("id")+")" + "您好：\n"+
                "欢迎光临"+CHINESE_MUD_NAME+"！\n"+
                "应您要求，您的密码已经发送至您的注册信箱。\n"+
                "请关闭自动登录功能，使用下面提供的密码登录。\n"+
                "\n"+
                ""+pass+ "\n"+
                "\n"+
                "用此密码登录成功后，建议您用 passwd 指令更换您的密码。\n"+
                "详情请在进入游戏后使用“help passwd”指令，获得相关信息。\n"+
                ctime(time());  
        
        write("开始发送通知信件到"+ ob->query("name")+ "(" + ob->query("id") + ")当前信箱。\n");
        SMTP_D->send_mail(this_player(),mail,""+CHINESE_MUD_NAME+"玩家自动找回密码信件",msg);       
        
        log_file("static/GETPASSWD", sprintf("%s 发送 %s 密码至注册信箱。 \n", this_player()->query("id"),ob->query("id")));
        destruct(ob);          

}

private int check_state(object ob)
{
        int day = 7;
        int ttime = 86400 * day;
        
        if (ttime < time() - ob->query("last_on"))
                return 1;
        else
                return 0;
}


int help(object me)
{
        write(@HELP
指令格式 : getpasswd [玩家ID帐号]

这个指令可以通过注册邮箱或者根据问题和答案重新发送登陆密码。

玩家自己发送遗忘帐号的密码回注册信箱，是指玩家一旦遗忘自己的某个帐号
在书剑游戏中的密码，并且确保该帐号在超过七天没有登陆，如果您可以正确
回答该帐号的注册信箱，那么系统会发送密码到该帐号的注册信箱。

如果该帐号已经设置问题保护，那么，您必须回答正确问题才可以发送密码。

HELP
);
        return 1;
}