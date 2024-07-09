
#include <ansi.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

//int sort_rank(object ob1,object ob2)
//{
//	reset_eval_cost(); // added by snowman@SJ
//	return ob1->query(va)-ob2->query(va);
//}


mixed main(object me,string arg)
{
    
    object *list;
    string str,k;
    int n,t,flag,v;
    int j;

    if(!arg) return notify_fail("你想要查看什么\n");
    sscanf(arg, "%s %d", k,v);
    
    str = "◎ " + MUD_NAME + " 状态统计\n\n";
    str += "帐号      姓名                 "+arg+"                                   \n";
    str += "——————————————————————————————————————————————————\n";
    list = users();
   // list = sort_array(list, (: $1->query($(arg))-$2->query($(arg)) :));
    j=sizeof(list);
    while( j-- ) {
     //   list[j]->delete("smart_fight");
        if(v) n=(list[j]->query(k)==v);
        else n=list[j]->query(arg);
        if(n) {
	    //    list[j]->delete(arg);
	        t++;
	        str +=  sprintf("%-9s %-10s %O\n",
	                list[j]->query("id"),
	                list[j]->name(1),
	                stringp(k)?list[j]->query(k):list[j]->query(arg)
	            );
	      }       
        
    }      
    str += "——————————————————————————————————————————————————\n";
    str = sprintf("%s共有 %d/%d 位使用者。\n", str, t,sizeof(list));

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

