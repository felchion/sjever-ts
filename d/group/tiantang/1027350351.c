// automake group room , Tue Jul 23 00:23:40 2002
inherit ROOM;
#include <ansi.h>
#include "/d/group/gate.h"
void create()
{
set("short","[1;33m宝物库[0;0m");
set("long","[1;36m一个十几米高的[0;33m大铁门(Tie men)[1;36m出现在我们面前,门上有
一道奇特的封印,这种封印只有天堂堂主才能打开。在墙上有个魔法做的光
球,在光球的照耀下,只见门内是一个足有足球场大小的石室,里面堆满了各
种各样的奇珍异宝,出里面的任何一样东西都价值连城。
[0;0m");
set("exits",([
"northwest":__DIR__"1027350275.c",
]));
set("objects",([
"/d/group/obj/qiangui.c":1,__DIR__"npc/1027354920.c":1,
]));
set("indoors","[0;35m人间[0;36m天堂[0;0m");
set("group1","tiantang");
set("group2","[0;35m人间[0;36m天堂[0;0m");
setup();
setup_var();
}
