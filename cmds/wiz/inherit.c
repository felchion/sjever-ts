//inherit by Ciwei@SJ

#include <ansi.h>

int help(object me);

static string file;

int check(object ob)
{
	if (  (strsrch(file_name(ob),"/d") == 0
	|| strsrch(file_name(ob),"/kungfu/class") == 0
	|| strsrch(file_name(ob),"/clone") == 0 )
	&& inherits(file,ob)!=0 ) return 1;
	return 0;
}

int cloned(object ob)
{
	return strsrch(file_name(ob),"#")!=-1;
}

string *colors = ({NOR"",HIG"",HIR""});

int main(object me,string arg)
{
	object *all;
	string str,temp;
		
	if(!arg) return help(me);
	if(file_size(arg)==-1) return notify_fail("目标不存在。\n");
	if(file_size(arg)==-2) return notify_fail("目标是个目录。\n");
	file = arg;
	all = objects( "check" );
	if(!sizeof(all)) return notify_fail("什么都没有发现。\n");
	str = "\n\t关于继承"+arg+"的相关信息：\n";
	str += sprintf("%-70s%-40s\n","物件","其他信息");
	str += "\n";
	for(int i =0;i<sizeof(all);i++){
		str += colors[inherits(arg,all[i])];
		if(cloned(all[i])) {
			temp = "为复制品";
			if(environment(all[i])) temp+= sprintf("，位置：%O",environment(all[i]));
			else temp += "。";
		}
		else temp = "非复制品。";
		str += sprintf("%-70O%-40s\n",all[i],NOR+temp );
		str += NOR;
	}
	me->start_more(str);
	return 1;	
}

int help(object me)
{
	write(@HELP
指令格式：inherit [文档]
	搜索所有继承该文档的物件。
	
	红色显示为需要Update的物件。
HELP
	);
	return 1;
}