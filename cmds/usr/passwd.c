// passwd.c

// Modified by mxzhao 2004/02/18
// Modified by LinuX  2004/02/19

inherit F_CLEAN_UP;

#include <database.h>
#include <ansi.h>
#include <mudlib.h>
#include <login.h>

string seed = "1AaBb2CcDd3EeFf4GgHh5IiJj6KkLl7MmNn8OoPp9QqRr0SsTtUuVvWwXxYyZz";

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

private void del_question(object me, string arg)
{
	mixed ret = dbquery("SELECT Question,Answer FROM Users "
		"WHERE U_Username="+save_variable(arg));
	
	if (!ret)
	{
		write("数据库连接失败。\n");
		return;
	}
	
	if (sizeof(ret) == 0)
	{
		write("数据库中没有发现 "+arg+" 这个玩家。\n");
		return;
	}
	
	if (ret[0][0]=="")
	{
		write("玩家 "+arg +" 没有设置问题和答案呢,清空什么呢?!\n");
		return;
	}
	
	if (!dbquery("update Users set Question = \"" + "" 
		+ "\",Answer = \""+""+"\" where U_username=\""+arg+"\""))
	{
		write(HIY"数据库连接失败。\n"NOR);
	}
	else
	{
		write("\n玩家 "+HIY+arg+NOR+" 在游戏中的问题和答案已经清空。\n");
	}	
}

private void get_question(object me, string arg)
{
	mixed ret = dbquery("SELECT Question,Answer FROM Users "
		"WHERE U_Username="+save_variable(arg));
	
	if (!ret)
	{
		write(HIY"数据库连接失败。\n"NOR);
		return;
	}
	
	if (sizeof(ret) == 0)
	{
		write("数据库中没有发现 " + arg + " 这个玩家。\n");
		return;
	}
	
	if (ret[0][0]=="")
	{
		write("玩家 " + arg + " 没有设置问题和答案呢!\n");
	}
	else
	{		
		write ("玩家 " + arg + " 游戏中的问题：" + ret[0][0] + "\n");	
		write ("玩家 " + arg + " 游戏中的答案：" + ret[0][1] + "\n");	
	}
}

int main(object me, string arg)
{
	if (me != this_player(1))
	{
		return 0;
	}


	if ( arg == "-change" )
	{
		write("E-mail地址改变程序！\n");
		write(HBRED+HIY"注意，设定的问题和答案永久不能更改，请小心设置！\n"NOR);
		write(HBRED+HIY"巫师以后不会帮你查找答案以及更换信箱，请务必记住您设定的问题和答案！\n"NOR);
		write("为了安全起见，请先输入您的登陆密码：");

		input_to("get_pass", me, me->name(1) );
	}
	else 
	{
		if (arg == "-c")
		{
			write("请输入您原来的确认密码(如未设置请直接按回车键)：");
		}
		else 
		{
			write("为了安全起见，请先输入您的登陆密码：");
		}
		
		input_to("get_old_pass", 1, me, arg);
	}

	return 1;
}

private void get_old_pass(string pass, object ob, string arg)
{
	mixed ret;
	if (arg == "-c")
	{
		string confirm_pass = ob->query("confirm_passwd");
		
		if (strlen(confirm_pass) == 0
			&& strlen(pass) > 0
			|| strlen(confirm_pass) > 0
			&& crypt(pass, confirm_pass) != confirm_pass)
		{
			write("游戏确认密码错误！\n");
			return;
		}
		
		write("请输入新的确认密码：");

		input_to("get_new_pass", 1, ob, arg);
	}
	else
	{
		string old_pass = PASSWD_D->get_passwd(ob->query("id"));
		
		if (crypt(pass, old_pass)!=old_pass && pass!=old_pass)
		{
			write("游戏登陆密码错误！\n");
			return;
		}
//		if ( arg == "-g")
//		{
//			string msg;
//			string name = getuid(ob);
//			
//			seteuid(getuid());
//			
//			ret = dbquery("select U_Email, Question, Answer from Users "
//				"where U_username=\""+ob->query("id")+"\"");
//			
//			if (!ret)
//			{
//				write("数据库连接失败。\n");
//				return;
//			}
//			
//			if (sizeof(ret) != 1)
//			{
//				write("数据库数据有问题，请与管理人员联系。\n");
//				return;
//			}
//			msg =   "" + ob->query("name") + "(" + ob->query("id")+")" + "您好：\n"
//				"欢迎光临" + CHINESE_MUD_NAME + "！\n"
//				"您的帐号修改信箱的提示问题是：" + ret[0][1] + "\n"
//				"答案是：" + ret[0][2] +"\n"
//				"修改信箱后，建议您妥善保护好自己的提示问题和答案，并保护好信箱密码。\n"
//				"详情请在进入游戏后后使用“help passwd”指令,获得相关信息。\n"
//				 + ctime(time()) + "\n";
//			write("开始发送信箱修改提示问题和答案信件到" + ob->query("name") + "(" + ob->query("id") + ")当前信箱。\n");
//			SMTP_D->send_mail(ob, name, "" + CHINESE_MUD_NAME + "信箱修改提示问题和答案信件", msg);
//			log_file("static/PASSWD", 
//				sprintf("%s get the question from IP: %s ", 
//				ob->query("id"),query_ip_number(this_player())));
//			return;
//				
//		}	
//		if ( arg == "-send" )
//		{
//			string new_pass;
//			string msg;
//			string name = getuid(ob);
//			
//			seteuid(getuid());
//			
//			if (!(new_pass = change_passwd(ob)))
//			{
//				write("取密码发生错误，请通知开发人员，暂时无法进行修改。\n");
//				return;
//			}
//			
//			msg =   "" + ob->query("name") + "(" + ob->query("id")+")" + "您好：\n"
//				"欢迎光临" + CHINESE_MUD_NAME + "！\n"
//				"您的确认密码已经更改为：\n\n" + new_pass + "\n\n"
//				"登录成功后，建议您用 passwd -c 指令更换您的确认密码。\n"
//				"详情请在进入游戏后后使用“help passwd”指令,获得相关信息。\n"
//				 + ctime(time()) + "\n";
//			
//			write("开始发送通知信件到" + ob->query("name") + "(" + ob->query("id") + ")当前信箱。\n");
//			SMTP_D->send_mail(ob, name, "" + CHINESE_MUD_NAME + "更换确认密码信件", msg);
//			return;
//		}
		else
		{
			write("请输入新的密码：");

			input_to("get_new_pass", 1, ob, arg);
		}
	}
}

private void get_new_pass(string pass, object ob, string arg)
{
	int i = strlen(pass);
	int j = 0;
	int k = 0;

	write("\n");

	if (pass == "")
	{
		write("放弃修改，继续使用原密码！\n");
		return;
	}
	
	if (i < 5)
	{
		write("密码的长度必须不小于五个字母，\n请重设您的密码：");
		input_to("get_new_pass", 1, ob, arg);
		return;
	}

	if (!LOGIN_D->query("md5") && i > 8)
	{
		write("密码的长度必须不大于八个字母，\n请重设您的密码：");
		input_to("get_new_pass", 1, ob, arg);
		return;
	}

	if (!wizardp(ob))
	{
		while(i--)
		{
			if( pass[i]>='0' && pass[i]<='9' )
			{
				j++;
			}
			
			if( pass[i]>='A' && pass[i]<='Z' )
			{
				k++;
			}
		}
		
		if (j < 1 || k < 1)
		{
			write("对不起，您的密码必须包含数字和英文大写字母。\n");
			write("请重设您的密码：");
			input_to("get_new_pass", 1, ob, arg);
			return;
		}
	}

	if (arg == "-c")
	{
		write("请再输入一次新的确认密码：");
	}
	else
	{
		write("请再输入一次新的密码：");
	}

	input_to("confirm_new_pass", 1, ob, crypt(pass, "$1$ShuJian"), arg);
}

private void confirm_new_pass(string pass, object ob, string new_pass, string arg)
{
	if( crypt(pass, new_pass) != new_pass )
	{
		write("您两次输入的新密码并不相同，继续使用原来的密码。\n");
		return;
	}

	if ( arg == "-c")
	{
		ob->set("confirm_passwd", new_pass);
		write("确认密码变更成功。\n");
		ob->save();
		return;
	}

	if (!PASSWD_D->set_passwd(ob->query("id"), new_pass))
	{
		write("密码变更失败、继续使用原密码！\n");
		return;
	}

	write("密码变更成功。\n");
}

private void get_pass(string pass, object ob, string name)
{
	string old_pass;
	mixed ret;
	
	old_pass = PASSWD_D->get_passwd(ob->query("id"));
	if( crypt(pass, old_pass)!=old_pass && pass!=old_pass )
	{
		write("密码错误！\n");
		return;
	}

	ret = dbquery("select U_Email, Question, Answer from Users "
		"where U_username=\""+ob->query("id")+"\"");
	
	if (!ret)
	{
		write("数据库连接失败。\n");
		return;
	}
	
	if (sizeof(ret) != 1)
	{
		write("数据库数据有问题，请与管理人员联系。\n");
		return;
	}
	
	ob->set("mail",ret[0][0]);
	
	write(HIG"\nE-mail保护是由您自己设定问题和答案，一旦您要改动自己的注册邮件地址，\n"
		"系统回询问您设定的问题。只有您给出的答案和设定的答案完全相同，才可以\n"
		"更改您的注册邮件地址。\n");

	if( ret[0][1] != "" )
	{
		write(HIW"\n请回答：" YEL + ret[0][1] + NOR);
		write(HIW"\n答  案："NOR);
		
		input_to("get_answer", ob ,ret[0]);
	} 
	else 
	{
		write(BLINK HIR"再次提醒：拥有安全问题答案可以随时更改你的注册邮箱！！\n"NOR 
			HIG"比如，安全问题可以是：自己的身份证号码是多少？\n"
			"答案设定成您的身份证号码。\n"
			"当您需要更改Email地址时，系统就会如下提示：\n"
			HIW"请回答安全设定问题："YEL"自己的身份证号码是多少？\n"
			HIW"答案：\n"
			HIG"这时，输入正确的答案。一旦答案正确，就可以更改自己的注册Email了。\n\n"NOR);
		
		write("你还没有设定自己的E-mail保护安全问题，\n请输入你要设定的问题：");

		input_to("new_question", ob );
	}
}

private void get_answer(string answer, object ob, mixed *res)
{
	string e_answer = res[2];
	if (answer != e_answer )
	{
		write("\n答案错误！请想清楚后再更改。\n");
		return;
	}

	write(HIG"\n答案正确！\n"NOR"您目前的注册地址是："+ res[0] +"\n");
	write("现在请输入您的新Email地址：");
	
	input_to("new_email", ob);
}

private void new_email(string email, object ob)
{
	string b_mail;
	int i;
	
	//string *forbid_email = ({
	//      "shujian.com","mymud.com","shujian.net"});
	
	if( !email || email == "" || email == " ")
	{
		write("放弃设定！下次请输入有效的"+HIG+"邮箱地址"+NOR+"！\n");
		return;
	}

	if ( strlen(email) > 30 )
	{
		write("对不起，邮件地址不能超过三十个字符。\n");
		return;
	}
	
	if( !email || sscanf(email, "%*s@%*s.%*s") != 3 )
	{
		write("您所输入的电子邮件地址格式错误！\n");
		return;
	}

	sscanf(email,"%*s@%s",b_mail);

/*	if (member_array(b_mail,forbid_email) != -1 )
 {
		write("对不起，此邮箱禁止作为注册邮箱。\n");
		return;
	}
*/	
	for(i = 0;i < sizeof(email);i++)
	{
		if (email[i] == '\"') 
		{
			write("输入错误，不能含有\"字符。\n");
			return;
		}
	}
	
	write(HIW"确认正确？[Y|N]"NOR);

	input_to("confirm_email", ob, email);
}

private void confirm_email(string arg, object ob, string email)
{
// Modified by mxzhao 2004/02/18
	string old_mail = ob->query("email");

	if( arg[0] != 'y' && arg[0] != 'Y' )
	{
		write("好吧，请下次想好了再更改。\n");
		return;
	}
	
	seteuid(getuid());
	
	if (!dbquery("update Users set U_Email= \"" + email 
		+"\" where U_username=\""+ob->query("id")+"\""))
	{
		write(HIY"数据库联系不上，暂时无法修改信箱，请与在线巫师联系。\n"NOR);
	}
	else
	{
		object link_ob;

		if( !ob->set("email", email) ) 
		{
			write("Email设定失败！请通知巫师！\n");
			return;
		}
		
		if ( old_mail != email ) 
		{
			log_file("static/PASSWD", 
				sprintf("%s change email from %s to %s IP: %s ", 
				ob->query("id"),old_mail,email,query_ip_number(this_player())));
		}
		
		ob->save();

		link_ob = new(LOGIN_OB);

		if (link_ob)
		{
			link_ob->set("id", ob->query("id"));
			link_ob->restore();
			link_ob->set("email", email);
			link_ob->save();

			destruct(link_ob);
		}
		
		write(HIG"\n成功！您的邮件地址已经更改为："+ email +" \n"NOR);
	}
// End
}

private void new_question(string question, object ob)
{
	int i;
	
	if( !question || question == "" || question == " ")
	{
		write("放弃设定！下次请输入有效的"HIG"问题"NOR"！\n");
		return;
	}
	
	if ( strlen(question) < 10 )
	{
		write("对不起，问题的长度不能少于十个字母！\n");
		return;
	}
	
	if ( strlen(question) > 40 )
	{
		write("对不起，问题的长度不能超过四十个字母！\n");
		return;
	}
	
	for(i = 0;i < sizeof(question);i++)
	{
		if (question[i] == '\"') 
		{
			write("输入错误，不能含有\"字符。\n");
			return;
		}
	}
	
	write("现在请输入答案（直接回车放弃答案设定）：");
	
	input_to("new_answer", ob, question);
}

private void new_answer(string answer, object ob, string question)
{
	int i;
	
	if( !answer || answer == "" || answer == " " )
	{
		write("放弃修改！下次请输入有效的"HIG"答案"NOR"！\n");
		return;
	}
	
	if ( strlen(answer) < 6 )
	{
		write("对不起，答案的长度不能少于六个字母！\n");
		return;
	}
	
	if ( strlen(answer) > 40 )
	{
		write("对不起，答案的长度不能超过四十个字母！\n");
		return;
	}
	
	for(i = 0;i < sizeof(answer);i++)
	{
		if (answer[i] == '\"')
		{
			write("输入错误，不能含有\"字符。\n");
			return;
		}
	}
	
	write("\n你设定提问是；" YEL + question + NOR 
		"\n答案是：" YEL + answer + HIW "\n正确吗？[Y|N]"NOR);

	input_to("confirm_change", ob, question, answer);
}

private void confirm_change(string arg, object ob, string question, string answer)
{
// Modified by mxzhao 2004/02/18
	if( arg[0] != 'y' && arg[0] != 'Y' )
	{
		write("操作取消。\n");
		return;
	}

	seteuid(getuid());

	if (!dbquery("update Users set Question = \"" + question 
		+ "\",Answer = \""+answer+"\" where U_username=\""+ob->query("id")+"\""))
	{
		write(HIY"数据库联系不上，暂时无法修改，请与在线巫师联系。\n"NOR);
	}
	else
	{
		write(HIG"\n你的提问和答案都设定成功！\n");
		log_file("static/PASSWD", 
			sprintf("%s set question and answer at %s  IP: %s ", 
			ob->query("id"),ctime(time()),query_ip_number(this_player())));

		write("您目前的注册地址是："+ ob->query("email") +"\n"NOR);
		write("现在请输入您的新Email地址（直接回车放弃设定）：");
		input_to("new_email", ob);
	}
// End
}

int help(object me)
{
	write(@HELP
指令格式 : passwd [-change]

这个指令可以修改你的人物密码。

-change  参数表示你要修改注册邮箱。
-c       设置或修改你的确认密码。
-send    发送新的确认密码到你的注册信箱。
-g       发送信箱修改提示问题和答案信件到你的注册信箱。

HELP

);
	return 1;
}

