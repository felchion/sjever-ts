/**********************************************************************************
//
//      dellease.c
//      Created by mychat 31/04/2004
//      保留我的签名情况下 free of use.
//      当然更欢迎修改、优化等。
//
//      Email & MSN: codemake@163.com
//      文件位于/cmds/adm下
// 		
***********************************************************************************/

//#include <roomlease.h>
#include <ansi.h>
#include <room.h>

#ifndef ROOMLEASE_D
#define ROOMLEASE_D "/adm/daemons/roomleased"
#endif

int main(object me, string arg)
{
	string roomid;
	string* leasemap=ROOMLEASE_D->query_keys();
	
	if( !arg || sscanf(arg, "%s", roomid)!=1 )
	{
		write("参数不齐!!\n");
		if(leasemap) write(sprintf("目前可用：%s\n",implode(leasemap,",")));
		return 1;		
	}

	if( !ROOMLEASE_D->query_ifhavebidname(roomid) )	
		return notify_fail(HIY"没有这个店铺要投标\n"NOR );
	return ROOMLEASE_D->delete_room(roomid);
}

int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：dellease 	roomid

--------------------------------------------------
该房间如果正在投标或出租就不能删除，必须用addlease将房间的属性修改为0，
禁止投标，然后无人投标和租用再删除。

write by mychat@ln(codemake@163.com)
--------------------------------------------------

HELP
    );
    return 1;
}
