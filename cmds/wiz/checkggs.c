
#include <ansi.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

mixed main(object me)
{
    
    object *list;
    string str;
    int t;
    int j;

    str = "◎ " + MUD_NAME + " 当前在线玩家鬼谷之术统计\n\n";
    str += "帐号            姓名       贵宾    永久变量     Condition     使用时间/剩余时间\n";
    str += "———————————————————————————————————————————\n";
    list = users();
    j=sizeof(list);
    while( j-- ) {
        if( !environment(list[j]) || !me->visible(list[j]) || !list[j]->query_temp("ggs/started") ) continue;
        t = time()-(int)list[j]->query("ggs/start_time");       
        str = sprintf("%s%-15s %-10s %-10s %-10s%-10s%s/%-20s\n",
                str,
                list[j]->query("id"),list[j]->name(1),
                (list[j]->query("registered")==3)?"是":HIR"否"NOR,
                (list[j]->query("ggs/started"))?"正常":HIR"异常"NOR,
                list[j]->query_condition("guigushu")?"正常":HIR"异常，请与LinuX联系"NOR,
                CHINESE_D->chinese_time(t),
                CHINESE_D->chinese_time(list[j]->query("ggs/left_time") -t)
            );
    }      
    str += "———————————————————————————————————————————\n";
    str = sprintf("%s共有 %d 位使用者连线中，系统负担：%s\n", str, sizeof(list),
        query_load_average() + "\n");

    write(str);
    return 1;

}

int help()
{
write(@HELP
指令格式 : checkggs

这个指令可以列出所有在线上的玩家的鬼谷术状态，可以查看有没有BUG。

相关指令： finger, who
HELP
    );
    return 1;
}

