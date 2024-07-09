
#include <ansi.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

mixed main(object me)
{
    
    object *list;
    string str;
    int t;
    int j;

    str = "◎ " + MUD_NAME + " 当前贵宾玩家状态统计\n\n";
    str += "帐号      姓名                 开始时间                                   到期时间\n";
    str += "——————————————————————————————————————————————————\n";
    list = users();
    j=sizeof(list);
    while( j-- ) {
        if( !environment(list[j]) || !me->visible(list[j]) || list[j]->query("registered")<3 ) continue;
        t = list[j]->query("vip/vip_time") - time();     
        str += sprintf("%-9s %-10s %-50s%-50s\n",
                list[j]->query("id"),
                list[j]->name(1),
                CHINESE_D->chinese_date(list[j]->query("vip/vip_start_time"),1),                
                CHINESE_D->chinese_time(t)
            );
        
    }      
    str += "——————————————————————————————————————————————————\n";
    str = sprintf("%s共有 %d 位使用者连线中，系统负担：%s\n", str, sizeof(list),
        query_load_average() + "\n");

    write(str);
    return 1;

}

int help()
{
write(@HELP
指令格式 : checkvip

这个指令可以列出所有在线上的玩家的贵宾状态，可以查看有没有BUG。

相关指令： finger, who
HELP
    );
    return 1;
}

