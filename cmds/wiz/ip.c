//ip.c


inherit F_CLEAN_UP;

#include <ansi.h>
#include <net/dns.h>
int help(object me);

void create(){  seteuid(getuid());}
private int check_multilogin();

int main(object me,string arg)
{
	string status;

	status = wizhood(me);
	if( me!=this_player(1) ||wiz_level(me) <wiz_level(status) )
	return notify_fail("你没有权力使用这个指令。\n");
	if(arg)
	return help(me);
	return check_multilogin();
}

private int check_multilogin()
{
	object *ob;
	string * ip_addr;
	string * temp;
	mixed * multy;
	int * count;
	string str,t;
	int i,j,s,s1;
	ob=users();
	s = sizeof(ob);
	ip_addr=allocate(s);
	ip_addr[0]=query_ip_number(ob[0]);
	temp=allocate(1);
	for (i=0;i<s;i++) {
		if( !environment(ob[i]) ) continue;
		temp[0]=query_ip_number(ob[i]);
		ip_addr=ip_addr-temp;
		ip_addr=ip_addr+temp;
	}
	ip_addr=ip_addr-({0});
	s1=sizeof(ip_addr);
	count=allocate(s1);
	multy=allocate(s1);
	for (i=0;i<s1;i++)
	multy[i]=allocate(s);
	for (i=0;i<s;i++) {
		t=query_ip_number(ob[i]);
		for (j=0;j<s1;j++)
		if (ip_addr[j]==t)  {
			if (ob[i]->query("id")){				 
				multy[j][count[j]]=ob[i]->query("name")+"("+capitalize(ob[i]->query("id"))+")["+query_ip_rport(ob[i])+"]";
				multy[j][count[j]]=sprintf("%-20s",multy[j][count[j]]);
				count[j]++;
			}
		}
	}
	for (i=0;i<s1;i++)
	multy[i]=multy[i]-({0});
	str=sprintf("%-17s%-4s\t%s\n","IP","人数","玩家");
	for (i=0;i<s1;i++) {
		if(count[i]>2)
		str+=sprintf(HIR"%-17s%4d\t"NOR,ip_addr[i],count[i]);
		else
		str+=sprintf(HIG"%-17s%4d\t"NOR,ip_addr[i],count[i]);
		for (j=0;j<sizeof(multy[i]);j++)
		str+=sprintf("%s ",multy[i][j]);
		str+=sprintf("\n");
	}
	str+=sprintf ("\n现在共有"+ HIC +"%d"+ NOR +"个玩家，",s);
	str+=sprintf ("从"+HIC+"%d"+ NOR +"个IP登录。\n\n",s1);
	this_player()->start_more(str);
	return 1;
}

int help(object me)
{
        write(@TEXT

指令格式：ip

这个指令是让你检查目前游戏中共有多少个相同的IP，相关
指令：mcheck。

TEXT);
        return 1;
}

