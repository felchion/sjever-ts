// ansi.c

#include <ansi.h>

inherit F_CLEAN_UP;

//string frame = "▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n";
string frame = "┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n";

string pattern = "①②③";

int main(object me)
{
	string *bgcolor = ({BBLK, BRED, BGRN, BYEL, BBLU, BMAG, BCYN, BWHT});
	string *fgcolor = ({BLK,RED,GRN,YEL,BLU,MAG,CYN,WHT,HIR,HIG,HIY,HIB,HIM,HIC,HIW});
	string *nfgcolor = ({"BLK","RED","GRN","YEL","BLU","MAG","CYN","WHT","HIR","HIG","HIY","HIB","HIM","HIC","HIW"});
	string output;
	int i,n;

	output = MUD_NAME+"ANSI颜色状态测试：\n";
	output += frame;
	output += "     BBLK  BRED  BGRN  BYEL  BBLU  BMAG  BCYN  BWHT\n";
	for(i=0;i<sizeof(fgcolor);i++)
	{
		output += sprintf("%-4s",nfgcolor[i]);
		for(n=0;n<sizeof(bgcolor);n++)
			output += sprintf("%s%s%s"+NOR,fgcolor[i],bgcolor[n],pattern);
		output += NOR+"\n";
	}
	output += NOR+frame;
	write(output);
        return 1;
}
int help(object me)
{
        write(@HELP
指令格式 : ansi

这个指令用来检验各种ANSI颜色和状态在正在使用的机器及软件
中的效果的。

HELP
        );
        return 1;
}