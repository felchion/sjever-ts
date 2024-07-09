#include <ansi.h>

int main(object me, string arg)
{
	if (!arg) return notify_fail("请输入汉字。\n");

	if (strlen(arg) > 20) return notify_fail("太多了。\n");
	write("\n"+HIY+CHINESE_D->font(arg)+NOR);
	write("\n"+HIY+CHINESE_D->chinese_graph_font(arg)+NOR);
	return 1;
}
