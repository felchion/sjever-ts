// color.c
// Modified by snowman@SJ 1999

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object me);
int main(object me, string arg)
{
	string color;
	object ob = me;

	if (!wizardp(me) && me->query("combat_exp") < 500000)
		return notify_fail("你的经验还不够给自己的名字增色，加油啊！\n");
	if (!wizardp(me) && me->query("color") &&
	me->query("color_time") >= me->query("combat_exp",1)/500000)
		return notify_fail("你不是加过颜色了吗？\n");
	if (!arg) return help(me);
	if (wizardp(me) && sscanf(arg, "%s %s", color, arg) == 2)
		ob = present(color, environment(me));
	if (!ob) return notify_fail("这里没有这个人。\n");

	switch (arg) {
		case "RED" : color = RED; break;
		case "GRN" : color = GRN; break;
		case "YEL" : color = YEL; break;
		case "BLU" : color = BLU; break;
		case "MAG" : color = MAG; break;
		case "CYN" : color = CYN; break;
		case "WHT" : color = WHT; break;
		case "HIR" : color = HIR; break;
		case "HIG" : color = HIG; break;
		case "HIY" : color = HIY; break;
		case "HIB" : color = HIB; break;
		case "HIM" : color = HIM; break;
		case "HIC" : color = HIC; break;
		case "HIW" : color = HIW; break;
		default :
			return notify_fail("你要给名字加什么颜色？\n");
	}
	ob->set("color", color);
	write("加好了。\n");
	write("你已经使用了这个指令"+chinese_number(ob->add("color_time", 1))+"次。\n");
	return 1;
}

int help(object me)
{
	write("
指令格式 : color <颜色>
 
如果你有五十万经验，那么你可以给你自己响亮的名字加上一个颜色。
以后每得到五十万经验，就能为自己的名字换一次颜色。
例如目前的经验是1,500,000，那么就可以使用 color 指令三次。
请选择以下颜色：

RED - "RED"■"NOR"红色    HIR - "HIR"■"NOR"亮红色
GRN - "GRN"■"NOR"绿色    HIG - "HIG"■"NOR"亮绿色
YEL - "YEL"■"NOR"土黄色  HIY - "HIY"■"NOR"亮黄色
BLU - "BLU"■"NOR"深蓝色  HIB - "HIB"■"NOR"亮蓝色
MAG - "MAG"■"NOR"浅紫色  HIM - "HIM"■"NOR"粉红色
CYN - "CYN"■"NOR"蓝绿色  HIC - "HIC"■"NOR"天青色
WHT - "WHT"■"NOR"浅灰色  HIW - "HIW"■"NOR"亮白色
\n");
	return 1;
}
