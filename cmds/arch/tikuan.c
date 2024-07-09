/**********************************************************************************
//
//      tikuan.c
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
	mapping leasemap;	
	string roomid;
	int amount;
	int irepayamount;
		
	if (!arg || sscanf(arg, "%s %d", roomid, amount) != 2)
		return notify_fail("命令格式：tikuan <店铺代码> <数量(货币单位为银子)>\n");

	if (!wizardp(me) && me->query_temp("command_busy"))
                return notify_fail("你正忙着呢。\n");
        me->set_temp("command_busy",1);
        call_out("remove_busy", 3, me);
	
	if( !ROOMLEASE_D->query_ifhavebidname(roomid) )	
		return notify_fail(HIY"没有这个出租的店铺。\n"NOR );

	leasemap=ROOMLEASE_D->query_amap(roomid);
	
	if( leasemap["leaser"]!=me->query("id") )
		return notify_fail(HIY"这个店铺不属于你，想混水摸鱼？\n"NOR );
	
	amount=amount*100;
	irepayamount=ROOMLEASE_D->sum_by_discount(leasemap["repayamount"],leasemap["discount"]);
	if( amount>(irepayamount-leasemap["payamount"]) )
		return notify_fail(HIY"店铺的收益还没这么多呢？\n"NOR );

	return ROOMLEASE_D->tikuan(me, roomid, amount);
	//return 1;
}

void remove_busy(object me)
{
        if (me) me->delete_temp("command_busy");
}


int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：tikuan
--------------------------------------------------
  从你承租的店铺受益中预支钱到存款中，但需要加收
  一定的手续费
  
--------------------------------------------------

HELP
    );
    return 1;
}
