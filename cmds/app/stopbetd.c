#include <ansi.h>
  
int main(object me, string arg)
{
	DOBET_D->stop_dobet();
	return 1;
}

int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：stopbetd
--------------------------------------------------
 stopbetd 停止赌局监护程序运行
--------------------------------------------------

HELP
    );
    return 1;
}
