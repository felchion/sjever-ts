// automake group room , Mon Jun 17 21:16:53 2002
inherit ROOM;
#include <ansi.h>
#include "/d/group/gate.h"
void create()
{
set("short","[1;37m沁[1;35m香[1;37m园[0;0m");
set("long","入得园中，只见佳木茏葱，奇花闪灼，一带清流，从花木深处曲折泻于石
隙之下。再进数步，渐向北边，平坦宽豁，两边飞楼插空，雕甍绣槛，皆隐于
山坳树杪之间。俯而视之，则[1;32m清溪[0;0m泻雪，石磴穿云，[1;37m白石[0;0m为栏，环抱池沿，石
桥三港，兽面衔吐。
　　当真是：[1;32m绕堤柳借三篙翠[0;0m，[1;37m隔岸花分一脉香[0;0m。
[0;0m");
set("exits",([
"up":__DIR__"1024296001.c","east":__DIR__"1024294381.c",
]));
set("objects",([
__DIR__"npc/1024318645.c":1,
]));
set("outdoors","[1;37m凝[1;35m香[1;37m阁[0;0m");
set("group1","baby");
set("group2","[1;37m凝[1;35m香[1;37m阁[0;0m");
setup();
setup_var();
}
