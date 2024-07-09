#include <ansi.h>
#include <mudlib.h>

private void confirm_passwd(string pass, object me, object ob);
int help(object me);

int LAST_DB_QUERY;

void confirm_transfer(string passwd,object me,string to,int tid,int amount,int tc) { 
	mixed ret;
	int uid;

	
	if(!PASSWD_D->check_passwd(me->query("id") ,passwd)) {
		write("\n密码错误！转账失败。\n");
		return;
	}

	ret = dbquery("SELECT extcredits2,uid FROM members WHERE username='"
	+me->query("id")+"@"+INTERMUD_MUD_NAME+"'");
	
	if(!ret){
		tell_object(me,"数据库通讯失败，请通知巫师。\n"); 
		return;
	}	
	if(sizeof(ret)<1){
		tell_object(me,HIR"您的论坛账号太长时间没有登录，无法同步数据。\n请登录论坛账号后再试。\n"NOR) ;
		return;
	}
	if(ret[0][0]<amount){
		tell_object(me,"你的元宝数量不够。\n"); 
		return;
	}	
	
	uid=ret[0][1];		
	log_cr(uid,-amount,tid);
	
	if(!dbquery("update members set extcredits2=extcredits2-"+amount+" WHERE uid="+uid)){		
		log_file("/static/transfer_error",sprintf("001: %s %d to tid=%d",me->query("id"),amount,tid));
		tell_object(me,"数据库通讯失败，请通知巫师。\n"); 
		return;
	}
 
	if(!dbquery("update members set extcredits2=extcredits2+"+amount+" WHERE uid="+tid)){		
		log_file("/static/transfer_error",sprintf("002: %s %d to tid=%d",me->query("id"),amount,tid));
		tell_object(me,"数据库通讯失败，请通知巫师。\n"); 
		return;
	}	
	
	tell_object(me,HIG"转账成功，你已累计转账"+(tc+amount)+"元宝！\n");
	return;
}

int main(object me, string arg)
{
	int flag,amount,tcount,pcount;
	object ob;
	string to;
	
	if (!me) return 0;
	
	if (!arg) return help(me);		 	
	
	if(sscanf(arg, "%d to %s", amount,to)==2) {
		mixed ret;
		int tid,tc;

		if(amount<1000 || amount>1000000 )
		return notify_fail("你一次只能转移一千到一百万个元宝。\n"); 
				
		if( time()-LAST_DB_QUERY <3 ) return notify_fail("系统繁忙，请稍后再试。\n");		
		LAST_DB_QUERY=time();
			
		ret = dbquery("SELECT extcredits2 FROM members WHERE username='"
			+ me->query("id")+"@"+INTERMUD_MUD_NAME+"'");
		
		if (!ret) 
		return notify_fail("数据库通讯失败，请通知巫师。\n"); 
		
		if (sizeof(ret)<1)
		return notify_fail(HIR"您的论坛账号太长时间没有登录，无法同步数据。\n请登录论坛账号后再试。\n"NOR) ; 
		
		if(ret[0][0]<amount)
		return notify_fail("你的元宝数量不够。\n"); 
		
		ret = dbquery("SELECT sum(extcredits2) from ultrax.common_credit_log where uid="+me->query("BBS_UID")+" and operation='TFR'");
		if (!ret || sizeof(ret)<1)
		return notify_fail("数据库通讯失败，请通知巫师。\n");
		
		tc = ret[0][0] + me->query("transfer_crlog");
		if( tc + (me->query("BBS_Credit_Used")/2 + me->query("SJ_TC_Used")*10) <  amount  ) {
			if(me->query("transfer_vip")) return notify_fail("你已累计转账"+-tc+"元宝。\n当前会员经验不足，暂时无法转账。\n");
			ret = dbquery("SELECT sum(extcredits2) from ultrax.common_credit_log where uid="+me->query("BBS_UID")+" and (operation='TFR' || operation='' || operation='TRC') ");
			if (!ret || sizeof(ret)<1)
			return notify_fail("数据库通讯失败，请通知巫师。\n");
			if( amount > ret[0][0]) return notify_fail("你已累计转账"+-tc+"元宝。\n当前会员经验不足，暂时无法转账。\n");
		}
		
		if(userp(ob=find_player(to))) {			
			if (!(tid=ob->query("BBS_UID"))) {				
				ret = dbquery("SELECT cast(uid as CHAR) FROM discuz6.uc_members WHERE username='"
				+to+"@"+INTERMUD_MUD_NAME+"'");
			  if (sizeof(ret)==1) {
			    sscanf(ret[0][0],"%d",tid);
			    ob->set("BBS_UID",tid);
			  }		
			}
		}
		else {
			ret = dbquery("SELECT cast(uid as CHAR) FROM discuz6.uc_members WHERE username='"
				+to+"'");
		  if (sizeof(ret)==1) 
		  	sscanf(ret[0][0],"%d",tid);
		  else 
		  	return notify_fail("找不到用户"+to+"，如用户在其它站点或不在线请用id@"+INTERMUD_MUD_NAME+"。\n"); 
		}
		
		write(HIR+"你要给"+to+HIR+"转账"+amount+"元宝，请输入账号密码确定："+NOR);
		input_to("confirm_transfer",me,to,tid,amount,-tc);
		return 1;
	}
	
	
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
	int tc;
	mixed ret;

	ret = dbquery("SELECT sum(extcredits2) from ultrax.common_credit_log where uid="+me->query("BBS_UID")+" and operation='TFR'");
	if (!ret || sizeof(ret)<1) {
		write("数据库通讯失败，请通知巫师。\n");
		return;
	}
	
	tc = ret[0][0] + me->query("transfer_crlog");
			
	confirm_pass = PASSWD_D->get_passwd(me->query("id"));
	
	if (!me || !ob) {
		write("转移会员身份失败！\n");
		return;
	}
	
	if ( crypt(pass, confirm_pass) != confirm_pass) {
		write("登录密码错误！\n");
		return;
	}
	
	log_file("transfer",sprintf("%s(%s) to %s(%s)，现:%d,通：%d/%d,原%d,累转%d。\n",
	  	me->query("name"),me->query("id"),ob->query("name"),ob->query("id"),
	  	me->query("vip/vip_time")/86400,me->query("SJ_Credit"),me->query("BBS_Credit_Used"),
	  	ob->query("BBS_Credit_Used"),tc
	  )
	);
	

	ob->set("vip/vip_time", me->query("vip/vip_time"));
	ob->set("vip/vip_start_time",time());	
	ob->set("transfer_coverd_credit",ob->query("BBS_Credit_Used"));
	ob->set("BBS_Credit_Used",me->query("BBS_Credit_Used")*9/10);
	ob->add("SJ_Credit",me->query("SJ_Credit"));	
	ob->set("introducer","会员转移精灵");	
	ob->set("transfer_time",me->query("transfer_time")+1);
	ob->add("transfer_crlog",tc);
	
	if(me->query("quest/vip_gift_lvl")>ob->query("quest/vip_gift_lvl"))
	ob->set("quest/vip_gift_lvl",me->query("quest/vip_gift_lvl"));
	
	ob->delete("env/transfer_accept");	
	me->add("transfer_vip",1); 
	me->set("registered",2);	
	me->delete("vip");
	me->delete("BBS_Credit_Used");
	me->delete("SJ_Credit");
	me->add("transfer_crlog_to",tc);
	
	
	tell_object(ob,HIY + me->query("name")+"把会员身份转让给你。" +
	"你现剩余贵宾时间:" + CHINESE_D->chinese_time(ob->query("vip/vip_time")-time()) +"。\n"NOR);

	write(HIY"转让会员身份成功！\n");

	return;
}

int help(object me)
{	write("\n指令格式 : transfer <ID> 或 <元宝数量> to <ID> \n");
	write(@HELP

这个指令可以让你把的白金以上会员和书剑通宝转
移到另一个帐号上，或将元宝转账给别一个帐号。

注意：转让会员将会损失百分之十通宝交易积分和
覆盖对方会员时间，并取消对方的邀请好友；转账
元宝累计数量不得超过会员经验值五倍；转账元宝
数量小于充值元宝数量时，转账不受限制。


HELP);

    return 1;
}
