//bbs.c 

#include <database.h>

int help(object me);

static object athor;

int main(object me,string arg)
{
	string db,sql;
	if(!arg) return help(me);
	if( sscanf(arg,"%s %s",db,sql)!=2 ) return help(me);
        BBS_D->add_Bbs_Up_Map(db,sql,this_object(),"back",1);
	athor = me;
	write("准备执行。。。。。。\n");
	return 1;
}

void back(mixed ret)
{
	if(athor) tell_object(athor,"你要求的SQL语句已经成功在WEB服务器上执行。\n");
	if(!sizeof(ret)) return;
	if(athor) tell_object(athor,sprintf("WEB服务器返回的结果是：%O",ret));
}


int help(object me)
{
	write(@HELP
	指令格式：bbs <dbname> <sql语句>
	功能可以在远程与MUD相连的WEB服务器上执行一条SQL语句。
HELP
	);
	return 1;	
}
