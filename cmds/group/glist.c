// glist.c  帮派列表
// by augx@sj 4/11/2002

#include <ansi.h>
inherit F_CLEAN_UP;
#include <mudlib.h>

int help(object me)
{
	write(@HELP
指令格式 :
glist
列出现有的帮派的情况。

glist [帮派id]
查看此帮派的成员。
HELP
	);
	return 1;
}

void remove_busy(object me)
{
	if (me) me->delete_temp("command_busy");
}

int list_member(string gid)
{
	mapping gmembers;
	string * sv, * sk;
	int i,j,k;

	if( !GROUP_D->site_now(gid) )
		return notify_fail( GROUP_D->get_last_error() );
	if( !(gmembers = GROUP_D->get_group(gid,2)) )
		return notify_fail("没有这个帮派！\n");

	sk = keys(gmembers);
	sv = values(gmembers);

	write("\n◎ "+CHINESE_MUD_NAME+"("+INTERMUD_MUD_NAME+") "+GROUP_D->get_group_name(gid)+"成员：("+HIG"绿色表示此人在线。"NOR+")\n");
	write("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");

	j = sizeof(gmembers);
	for(i=0,k=1;i<j;i++) {
		if( k == 1 ) write("┃");
		write( sprintf("  %-18s  ",(find_player(sk[i])?HIG:NOR)+sv[i]+"("+capitalize(sk[i])+")"+NOR) );
		k++;
		if( k > 4 ) {
			write("┃\n");
			k = 1;
		}
	}

	if( k>1 && k<5 ) write( sprintf("%"+((5-k)*22+2)+"s\n","┃") );
	write("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	write(GROUP_D->get_group_name(gid)+"现在总共有成员"+chinese_number(j)+"名。\n");

	this_player()->set_temp("command_busy", 1);
	call_out("remove_busy", 3, this_player());
	return 1;
}

int main(object me, string arg)
{
	mapping * groups, mp;
	int i;

	if( me->query_temp("command_busy"))
		return notify_fail("你正忙着呢。\n");

	if( arg ) return list_member(arg);

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

	me->set_temp("command_busy", 1);
	call_out("remove_busy", 3, me);
	return 1;
}
