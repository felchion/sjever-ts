
#include <ansi.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

mixed main(object me,string arg)
{
    
    object *list;
    string str;
    int n,t,flag;
    int j;

    if(!arg) return notify_fail("你想要查看什么\n");
    str = "◎ " + MUD_NAME + " 状态统计\n\n";
    str += "帐号      姓名                 "+arg+"                                   \n";
    str += "——————————————————————————————————————————————————\n";
    list = users();
    j=sizeof(list);
    while( j-- ) {
        n=list[j]->query_temp(arg);
        if(n && !list[j]->is_fighting()) {
	      //  list[j]->delete(arg);
	        str +=  sprintf("%-9s %-10s %O %O\n",
	                list[j]->query("id"),
	                list[j]->name(1),
	                list[j]->query_temp(arg),
	                sizeof(list[j]->query_temp(arg))
	            );
	      }       
        
    }      
    str += "——————————————————————————————————————————————————\n";
    //str = sprintf("%s共有 %d/%d 位使用者。\n", str, n,sizeof(list));

    me->start_more(str);
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

