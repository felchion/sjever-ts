
#include <ansi.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

mixed main(object me,string arg)
{
    
    object *list;
    string str,str2;
    int t;
    int j;
    int i;
	
    str = "◎ " + MUD_NAME + " 当前书剑玩家成长速度状态统计\n\n";
    str += "帐号      姓名                 上次经验          本次经验      平均速度(分/小时)       上线时间\n";
    str += "——————————————————————————————————————————————————\n";
    
    if(arg)
    	list = filter_array(users(), (: $1->query("id") == $(arg) :));
    else
    	list = users();
    j=sizeof(list);
    while( j-- ) {
        if( !environment(list[j]) 
        || !me->visible(list[j]) 
        || !interactive(list[j])  ) continue;
        
        if((uptime() - list[j]->query_temp("time"))<=0) continue;
        i = (list[j]->query("combat_exp") - list[j]->query_temp("combat_exp")) * 60
                / (uptime() - list[j]->query_temp("time"));
        str2 = sprintf("%d",i); 
        if (i < 20) continue;
        t = uptime() - list[j]->query_temp("time");
                  
        str += sprintf("%-9s%-23s%-16d %-16d%-3s／%-16d%s\n",
                list[j]->query("id"),
                list[j]->is_robot()?(HIG+list[j]->name(1)+NOR):list[j]->name(1),
                list[j]->query_temp("combat_exp"), 
                list[j]->query("combat_exp"),
                i>100?(HIR+str2+NOR):str2,
                i*60,
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

