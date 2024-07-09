// title.c (Mon  09-04-95)
// Modified by Ciwei@SJ
 
#include <ansi.h>
inherit F_CLEAN_UP;
 
int main(object me, string str)
{
	if(wizardp(me) && stringp(str)) me->set("title",str);
	write(sprintf(BOLD "%s" NOR "%s\n", RANK_D->query_rank(me), me->short(1)));
	return 1;
}
 
int help(object me)
{
	if(wizardp(me)) 
	write(@HELP
指令格式: title [参数]
 
显示你目前的阶级和头衔。

如果有参数则修改自己的头衔。
 
HELP
	);
	else write(@HELP
指令格式: title
 
显示你目前的阶级和头衔。
 
HELP
	);
}
