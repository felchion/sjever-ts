//getstr.c

inherit F_CLEAN_UP;

int main(object me,string str)
{
	if(!wizardp(me)) return 0;
	if(!str) return notify_fail("你搞毛啊？\n");
	str = replace_string(str,"\"","");
	write(sprintf("字符 [%s] 长度=%d\n",str,strlen(str)));
	return 1;
}
int help(object me)
{
	if(!wizardp(me)) return 0;
	write(@HELP
	格式：getstr 字符
	获得字符长度，巫师专用。
	
	一个建议：最好先关闭zMUD的自动解析功能再使用该指令。
		  或者在字符边上加""
HELP
);
	return 1;
}
