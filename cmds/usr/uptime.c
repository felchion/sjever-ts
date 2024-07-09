// uptime.c
// From ES2
// Modified by Yu Jue for JY (1998-3-17)

inherit F_CLEAN_UP;

#include <localtime.h>
#include <mudlib.h>

int main(int i)
{
	write("\n"+CHINESE_MUD_NAME + "已经连续执行了" + CHINESE_D->chinese_time(uptime()) + "。\n");
	
	if ( intp(i) || file_size("/log/static/LASTCRASH") <= 0) return 1;
	
	write("当机记录：" + read_file("/log/static/LASTCRASH"));
        
        return 1;
}

int help(object me)
{
        write(
"指令格式 : uptime\n\n"
"这个指令告诉您「" MUD_NAME "」泥巴游戏已经连续执行了多久。\n"
    );
    return 1;
}
