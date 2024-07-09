// ban.c

void create()
{
	seteuid(getuid());
}

int main(object me, string arg)
{
	string site;

	if (!arg)
		BAN_D->print();
	else if (sscanf(arg, "+ %s", site) == 1) {
		if (site[<1] == '*' ||
		    site[<1] == '?' ||
		    site[<1] == '+')
			write("不能禁以 *, +, ? 结尾的地址。\n");
		else
			BAN_D->add(site);
		}
	else if (sscanf(arg, "- %s", site) == 1)
		BAN_D->del(site);
	else write("指令格式：ban [+|- site]\n");

	return 1;
	
}

int help(object me)
{
	write(@HELP
指令格式：ban [+|- site]

这条命令是用来防止有人蓄意捣乱。
HELP
	);
	return 1;
}
