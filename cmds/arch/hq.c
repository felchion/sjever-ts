/**********************************************************************************
//
//      hq.c
//      Created by mychat 31/04/2004
//      保留我的签名情况下 free of use.
//      当然更欢迎修改、优化等。
//
//      Email & MSN: codemake@163.com
//      文件位于/cmds/usr下
// 		
***********************************************************************************/
#include <ansi.h>
//#include <roomlease.h>

#ifndef ROOMLEASE_D
#define ROOMLEASE_D "/adm/daemons/roomleased"
#endif

int main(object me, string arg)
{
	string outstr;
	
        if (!wizardp(me) && me->query_temp("command_busy"))
                return notify_fail("你正忙着呢。\n");
        me->set_temp("command_busy",1);
        call_out("remove_busy", 3, me);
        if(arg && arg=="me") outstr=ROOMLEASE_D->query_list(me->query("id"));
        else outstr=ROOMLEASE_D->query_list();
	if(!outstr)
		return notify_fail("没有任何出租店铺的信息！要不找wiz抗议去？\n");
	me->start_more( outstr );
	return 1;
}

void remove_busy(object me)
{
        if (me) me->delete_temp("command_busy");
}


int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：hg [me]
--------------------------------------------------
  看当前投标的行情
  使用参数me表示查看跟自己相关的行情
  
--------------------------------------------------

HELP
    );
    return 1;
}
