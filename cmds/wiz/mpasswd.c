// mpasswd.c
// by Find.

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	int cap, low, num, spec, n, len, sz;
	string passwd;
	string Random_Passwd ="ABCasdh3aejbnDEFGHIJKL1234567890mnopqr5stHJGHJHJuvwxyza2454bcdefghi1jkl9MNOPQRS4TUVWytu9XYZ";

	len = atoi(arg);

	if( (len < 12) || (len > 20) )
		return notify_fail("随机密码只能 12-20 位。\n");

	sz = sizeof(Random_Passwd);

	do
	{
		cap=0; low=0; num=0; passwd = "";
		for(int i=0;i<len;i++)
		{
			n = random(sz);
			if( Random_Passwd[n] <='Z' && Random_Passwd[n] >='A' )
				cap++;
			else if( Random_Passwd[n] <='z' && Random_Passwd[n] >='a' )
				low++;
			else if( Random_Passwd[n] <='9' && Random_Passwd[n] >='0' )
				num++;
			else
				spec++;
			passwd += Random_Passwd[n..n];
		}
	}
	while(!cap || !low || !num);

	printf("你希望的 %d 位随机密码如下：\n%s\n\n", len, passwd);

	return 1;
}

int help(object me)
{
	write(@HELP
指令格式: mpasswd 随机密码的位数

这个指令帮助巫师生成一个指定位数，符
合巫师密码要求的随机密码。
允许的位数为 12-20 位。

HELP
	);
	return 1;
}
