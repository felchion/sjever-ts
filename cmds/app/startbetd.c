#include <ansi.h>
  
int main(object me, string arg)
{
	DOBET_D->start_dobet();
	return 1;
}

int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：startbetd
--------------------------------------------------
 startetd 恢复赌局监护程序运行
--------------------------------------------------

HELP
    );
    return 1;
}
