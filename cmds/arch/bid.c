/**********************************************************************************
//
//      bid.c
//      Created by mychat 31/04/2004
//      保留我的签名情况下 free of use.
//      当然更欢迎修改、优化等。
//
//      Email & MSN: codemake@163.com
//      文件位于/cmds/usr下
// 		
***********************************************************************************/
#include <ansi.h>
#ifndef ROOMLEASE_D
#define ROOMLEASE_D "/adm/daemons/roomleased"
#endif
//#include <roomlease.h>
int main(object me, string arg)
{
//	int    oldbidamount;		
	string bidname,flag;
	int bidamount,mybalance,bidflag;
	string valroomname;
	mapping leasemap;
	
	bidflag=1;
	if( !stringp(arg)) 
		return notify_fail("你投什么标？\n");
	if( (sscanf(arg, "%s %d %s", bidname, bidamount,flag)==3)&&(flag=="-t") ){
		bidflag=2;
	}else if( sscanf(arg, "%s %d", bidname, bidamount)==2 );
	else return notify_fail("你投什么标？\n");
	
	if (!wizardp(me) && me->query_temp("command_busy"))
                return notify_fail("你正忙着呢。\n");
        me->set_temp("command_busy",1);
        call_out("remove_busy", 3, me);
 
	if( !ROOMLEASE_D->query_ifhavebidname(bidname) )	
		return notify_fail(HIY"没有这个店铺要投标\n"NOR );

	leasemap=ROOMLEASE_D->query_amap(bidname);
	
	if( (leasemap["bidtype"]!=1)&&(leasemap["bidtype"]!=2) )
		return notify_fail(HIY"这个店铺还没打算投标\n"NOR );

	mybalance=me->query("balance");
	//投标时用的silver为单位
	bidamount=bidamount*100;
	
	valroomname=ROOMLEASE_D->get_myregions(leasemap["roomwhere"])+leasemap["originshortname"];

	write( HIY"你填写了一张" + 
		ROOMLEASE_D->money_str(bidamount)+
		"的支票，想要投标【"+
		valroomname + 
		"】"+
		ROOMLEASE_D->chinese_time(leasemap["leasetime"]) +
		"的租用权\n"NOR );
	if(mybalance<bidamount)
		return notify_fail(HIY"但是你没这么多存款\n"NOR);
	call_out("do_bid",3+random(3),me,bidname, bidamount,bidflag);
	return 1;
}

void do_bid(object me,string bidname,int bidamount,int bidflag)
{
	mapping leasemap=ROOMLEASE_D->query_amap(bidname);
	
	me->set_temp("command_busy",1);
        call_out("remove_busy", 3, me);
                
	if(leasemap["leaser"]==me->query("id"))
	{
		tell_object(me,"你是不是犯傻了？这已经是你的店了。\n");
		return;
	}
	if(time()<leasemap["bidtime"])
	{
		tell_object(me,HIY"但是投标还没有开始呢，请稍后再来吧\n"NOR);
		return;
	}
	if(time()>leasemap["bidendtime"])
	{
		tell_object(me,HIY"但是当前投标已经结束，请稍后再来吧\n"NOR);
		return;
	}
	if(me->query("id")==leasemap["bider"])
	{
		tell_object(me,HIY"当前最高价就是你出的，你钱多烧得慌，还要自己再加价？\n"NOR);
		return;
	}
	if(leasemap["minirent"]>bidamount)
	{
		tell_object(me,HIY"但是你出的价必须比最低入围价高\n"NOR);
		return;
	}
	if(leasemap["bidamount"]>bidamount)
	{
		tell_object(me,HIY"但是你出的价还没当前最高价高\n"NOR);
		return;
	}
	if( (leasemap["bidamount"]+leasemap["minirentadd"])>bidamount )
	{
		tell_object(me,sprintf(HIY"但是加价的最低幅度为%d\n",leasemap["minirentadd"]));
		return;
	}
	if(ROOMLEASE_D->new_bid(me, bidname,bidamount,bidflag)) return;
	tell_object(me,"投资失败！\n");
}

void remove_busy(object me)
{
        if (me) me->delete_temp("command_busy");
}


int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：bid 投标代码 投标金额 <-t>
--------------------------------------------------
 投标金额:以两银子为单位
 -t 以匿名身份投标
 
--------------------------------------------------

HELP
    );
    return 1;
}
