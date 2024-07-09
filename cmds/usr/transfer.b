#include <ansi.h>

private void confirm_passwd(string pass, object me, object ob);
int help(object me);

int main(object me, string arg)
{
	int flag;
	object ob;
	
	if (!me) return 0;
	
	if (!arg) return help(me);
		 	
	ob = find_player(arg);
		
	if (!ob) 
		return notify_fail("这个用户不在线，无法转让！\n");
	
	if (!userp(ob)) 
		return notify_fail(ob->query("ID")+"是什么东东！\n");		
	
	if (ob==me) 
		return notify_fail("转给自己？\n");
		
	if(!ob->query("env/transfer_accept"))
		return notify_fail("请通知对方先使用 set transfer_accept 1 命令设置接受转让。\n");
	
	if(me->query("registered")<4 )
		return notify_fail("只有白金以上才能转让身份。\n");

	if(me->query("registered")>9 && !me->query("transvip_confirm"))
		return notify_fail("青金以上会员转让，须经管理员确认。\n");
				
	if(me->query("transfer_time")>me->query("registered")-3)
		return notify_fail("你的会员等级太低无法转让。\n");
	
	if(me->query("BBS_Credit_Used") < ob->query("BBS_Credit_Used"))
		return notify_fail("你只能把会员身份转让通宝交易积分比你低的用户。\n");		


	write(HIR"注意：转让会员将会损失百分之十通宝交易积分和覆盖对方会员时间及交易积分。\n"+
	"对方的推荐好友将会被取消。\n"+
	"按回车键取消，输入登录密码确定。请输入登录密码：\n"NOR);
	input_to("confirm_passwd", me,ob);
	return 1;
}

private void confirm_passwd(string pass, object me, object ob) 
{
	string confirm_pass;
	
	confirm_pass = PASSWD_D->get_passwd(me->query("id"));
	
	if (!me || !ob) {
		write("转移会员身份失败！\n");
		return;
	}
	
	if ( crypt(pass, confirm_pass) != confirm_pass) {
		write("登录密码错误！\n");
		return;
	}
	
	log_file("transfer",sprintf("%s(%s) to %s(%s)，现:%d,通：%d/%d,原%d。\n",
	  	me->query("name"),me->query("id"),ob->query("name"),ob->query("id"),
	  	me->query("vip/vip_time")/86400,me->query("SJ_Credit"),me->query("BBS_Credit_Used"),
	  	ob->query("BBS_Credit_Used")
	  )
	);
	

	ob->set("vip/vip_time", me->query("vip/vip_time"));
	ob->set("vip/vip_start_time",time());	
	ob->set("transfer_coverd_credit",ob->query("BBS_Credit_Used"));
	ob->set("BBS_Credit_Used",me->query("BBS_Credit_Used")*9/10);
	ob->add("SJ_Credit",me->query("SJ_Credit"));	
	ob->set("introducer","会员转移精灵");	
	ob->set("transfer_time",me->query("transfer_time")+1);
	
	if(me->query("quest/vip_gift_lvl")>ob->query("quest/vip_gift_lvl"))
	ob->set("quest/vip_gift_lvl",me->query("quest/vip_gift_lvl"));
	
	ob->delete("env/transfer_accept");	
	me->add("transfer_vip",1); 
	me->set("registered",2);	
	me->delete("vip");
	me->delete("BBS_Credit_Used");
	me->delete("SJ_Credit");
	
	
	tell_object(ob,HIY + me->query("name")+"把会员身份转让给你。" +
	"你现剩余贵宾时间:" + CHINESE_D->chinese_time(ob->query("vip/vip_time")-time()) +"。\n"NOR);

	write(HIY"转让会员身份成功！\n");

	return;
}

int help(object me)
{	write("\n指令格式 : transfer <ID>\n");
	write(@HELP

这个指令可以让你把的白金以上会员和书剑通宝转
移到另一个帐号上。

注意：转让会员将会损失百分之十通宝交易积分和
覆盖对方会员时间，并取消对方的邀请好友。

HELP);

    return 1;
}
