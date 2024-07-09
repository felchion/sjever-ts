
#include <ansi.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

mixed main(object me)
{
    
    object *list;
    string str,logs=read_file("log/cut.log");
    int t;
    int j;

    str = "◎ " + MUD_NAME + " 当前存款玩家状态统计\n\n";
    str += "帐号      姓名                 礼物时间                                   存款\n";
    str += "——————————————————————————————————————————————————\n";
    list = users();
    
    list = filter_array(users(),
			(: $1->query("spec/cbook") > 1 && $1->query("/spec/last_week")==2145 :)
		);
    
    j=sizeof(list);
    while( j-- ) {
       if (list[j]->query("spec/cbook")==1) {
       list[j]->set("spec/cbook",99);
       list[j]->add("quest/chance",-1);
       list[j]->add("SJ_Credit",20);
      }
       if (list[j]->query("spec/cbook")==98) {
       	list[j]->set("spec/cbook",97);
       	list[j]->add("quest/chance_times",-1);
	      list[j]->add("job_time/丢失的信",200);
	     }
       if (list[j]->query("quest/SJ_Credit") == 20){
        list[j]->delete("quest/SJ_Credit");
        list[j]->add("SJ_Credit",20);
       }
        
        str += sprintf("%-9s %-10s %-40d%-46d %3d\n",
                list[j]->query("id"),
                list[j]->name(1),
                list[j]->query("quest/SJ_Credit"),                
                list[j]->query("spec/cbook"),
                list[j]->query("quest/chance_times")
            );
        
    }      
    str += "——————————————————————————————————————————————————\n";
    str = sprintf("%s共有 %d 位使用者连线中，系统负担：%s\n", str, sizeof(list),
        query_load_average() + "\n");

    this_player()->start_more(str);
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

