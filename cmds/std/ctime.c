// ctime.c

int main(object me, string arg)
{
	int t;

	seteuid(geteuid(me));
	if (!arg || sscanf(arg,"%d",t)<1) return notify_fail("指令格式 : ctime <时间>\n");
	tell_object( me , sprintf("%s = %s\n",arg,ctime(t)) );
        return 1;
}

int help(object me)
{
write(@HELP
指令格式 : ctime <时间>
 
转换时间格式。
HELP
    );
    return 1;
}
