/**********************************************************************************
//
//      stoplease.c
//      Created by mychat 31/04/2004
//      保留我的签名情况下 free of use.
//      当然更欢迎修改、优化等。
//
//      Email & MSN: codemake@163.com
//      文件位于/cmds/adm下
// 		
***********************************************************************************/
#include <ansi.h>
#include <room.h>
//#include <roomlease.h>

#ifndef ROOMLEASE_D
#define ROOMLEASE_D "/adm/daemons/roomleased"
#endif
  
int main(object me, string arg)
{
	if(arg=="-s")
	{
		if(ROOMLEASE_D->start_lease()) write("商业运营机制正常恢复！\n");
		else write("恢复失败 或者 未曾关闭！\n");
		return 1;
	}
	ROOMLEASE_D->stop_lease();
	write("商业运营机制停止运行！\n");
	return 1;
}

int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：stoplease [-s]
	-s 表示重新开始
--------------------------------------------------
 stoplease 停止roomleased的callouts,以便update

write by mychat@ln(codemake@163.com)
--------------------------------------------------

HELP
    );
    return 1;
}
