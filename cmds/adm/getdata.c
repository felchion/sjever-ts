
int help(object me);

int main(object me,string arg)
{
	mixed ret;	
	if(!arg) return help(me);
	ret = dbquery("SELECT U_Data FROM Data WHERE U_ID=" 
		+ save_variable(arg));	
	write(arg+"\n");
	if(!ret){
		return notify_fail("读取数据库失败。\n");
	}
	if(sizeof(ret)>0)
	{
		for(int i =0;i<sizeof(ret);i++){
			if(bufferp(ret[0][0])) arg = read_buffer(ret[0][0]);
			else arg = ret[0][0];
			write(sprintf("资料%d：%s\n",i,arg));
		}
		return 1;
	}
	write("没有读取到任何资料。\n");
	return 1;
}
int help(object me)
{
write(@HELP
	getdata [string]
	例如：
	getdata /data/login/c/ciwei.o
HELP
);
	return 1;	
}