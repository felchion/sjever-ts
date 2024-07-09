// glist.c  帮派列表
// by augx@sj 4/11/2002
//modified by tangfeng@sj 8/7/2004

#include <ansi.h>
inherit F_CLEAN_UP;
#include <mudlib.h>

#if !defined(GROUP_D)
#define GROUP_D   "/adm/daemons/groupd"
#endif

int main(object me, string arg)
{
	mapping * groups, mp;
	mapping gmembers;
	string gid;
	string * sv, * sk, * option;
	object ob;
	int i,j,k,p=0;
  int opt_long,opt_inter;
  
  if( me->query_temp("command_busy"))
		return notify_fail("你正忙着呢。\n");

	if(!arg )
	{
  	write("\n◎ "+CHINESE_MUD_NAME+"("+INTERMUD_MUD_NAME+") 帮派排行榜：\n");
	  write("┏━━┯━━━━━━┯━━━━┯━━━━━━━━━┯━━━┯━━━┯━━━┯━━━━┯━━━━┓\n");
	  write("┃排名│  帮派名称  │帮派标识│      帮  主      │帮众数│实  力│名  望│当前站点│迁移站点┃\n");
	  write("┠──┴──────┴────┴─────────┴───┴───┴───┴────┴────┨\n");

	  groups = GROUP_D->get_sort_group();
	  for(i=0;i<sizeof(groups);i++) {
		  mp = GROUP_D->get_group(groups[i]["id"]);
		  write( sprintf("┃%-4i  %-12s  %-8s  %-18s  %2i(%2i)  %-6i  %-6i  %-8s  %-8s┃\n",
			  i+1,mp["name"],groups[i]["id"],mp["master1_name"]+"("+mp["master1"]+")",mp["members"],mp["max_members"],mp["power"],mp["score"],mp["site"],mp["site_next"]) );
	  }

	  write("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	  write("当前活动站点不是本站的帮派的统计数据每天早晨更新一次。\n");
	  return 1;
	}
	else
	{
    //引自who.c@sj
	  option = explode(arg, " ");
	  option -= ({ "" });	
	  //这个的意思是？
	  i = sizeof(option);
	  while( i-- )
			switch(option[i]) {
				case "-l": opt_long = 1; break;
				case "-!": opt_inter = 1; break;
				default:
        /*
        //不是很了解网络设置
					if( option[i][0]=='@' ) {
						notify_fail("网路精灵并没有被载入。\n");
						if (find_object(DNS_MASTER) && RWHO_Q->send_rwho_q(option[i][1..<1],
							me, opt_long & wizardp(me), opt_inter)) {
							// 只有巫师才允许 who @xx -l，Yu Jue@SJ 19990624
							write("网络讯息已送出，请稍候。\n");
							return 1;
						}
						return 0;
					}
        */
          if(!option[i])
		        return notify_fail("没有说明需要查询的帮派名称！\n");	      
	        if( !GROUP_D->site_now(gid=option[i]) )
		        return notify_fail( GROUP_D->get_last_error() );
	        if( !(gmembers = GROUP_D->get_group(gid,2)) )
		        return notify_fail("没有这个帮派或者参数设置错误！\n");	      
			}
	  }
 
  	sk = keys(gmembers);
  	sv = values(gmembers);
    if (opt_inter)
    	write("\n◎ "+CHINESE_MUD_NAME+"("+INTERMUD_MUD_NAME+") "+GROUP_D->get_group_name(gid)+"成员：("+HIG"绿色表示此人在线。"NOR+")\n");
    else 
    	write("\n◎ "+CHINESE_MUD_NAME+"("+INTERMUD_MUD_NAME+") "+GROUP_D->get_group_name(gid)+"在线成员"NOR+"\n"); 
  	write("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
  
    if (opt_long)
    {
      j = sizeof(gmembers);		
	    for(i=0;i<j;i++) {
		     write("┃");
  	     if(find_player(sk[i])) p++;
		     if(opt_inter && !(ob=find_player(sk[i]))) continue;
		     write( sprintf("%12s%s%s\n",GROUP_D->get_member_class(sk[i]),find_player(sk[i])?	HIG"*"NOR:" ",ob->short(1)));
		     //这里自己调整需要显示的内容；建议显示帮派的级别，帮主或副帮主
		     write("┃\n");		
		  }
	    if(p=0 && opt_inter)
      {
        write(HIG+GROUP_D->get_group_name(gid)+"现在总共有成员"+chinese_number(j)+"名;却无人在线！\n");
   	    write("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
      }
      else  
	    {
	      write("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	      write(HIG+GROUP_D->get_group_name(gid)+"现在总共有成员"+chinese_number(j)+"名;"+chinese_number(p)+"在线！\n");
      } 
    }
    else 
    {
	    j = sizeof(gmembers);
	    for(i=0,k=1;i<j;i++) {
		    if( k == 1 ) write("┃");
		    if(find_player(sk[i])) p++;
		    if(opt_inter && !(ob=find_player(sk[i]))) continue;
		    write( sprintf("  %-18s  ",(find_player(sk[i])?HIG:NOR)+sv[i]+"("+capitalize(sk[i])+")"+NOR) );
		    k++;
		    if( k > 4 ) {
			     write("┃\n");
			     k = 1;
		    }
	   }
     if(p=0 && opt_inter)
     {
        write(HIG+GROUP_D->get_group_name(gid)+"现在总共有成员"+chinese_number(j)+"名;却无人在线！\n");
   	    write("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
     }
     else  
	   {
	     if( k>1 && k<5 ) write( sprintf("%"+((5-k)*22+2)+"s\n","┃") );
	     write("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	     write(HIG+GROUP_D->get_group_name(gid)+"现在总共有成员"+chinese_number(j)+"名;"+chinese_number(p)+"在线！\n");
     }
  }
	
	me->set_temp("command_busy", 1);
	call_out("remove_busy", 3, me);
	return 1;
}

void remove_busy(object me)
{
	if (me) me->delete_temp("command_busy");
}
int help(object me)
{
	write(@HELP
指令格式 :
glist 列出现有的帮派的情况。
glist [帮派id] 查看此帮派的成员。
glist -l 查看详细玩家数据。
glist -! 查看在线帮派中人。
HELP
	);
	return 1;
}
