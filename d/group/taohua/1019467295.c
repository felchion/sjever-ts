// automake group room , Mon Apr 22 17:22:51 2002
inherit ROOM;
#include <ansi.h>
#include "/d/group/gate.h"
void create()
{
set("short","石路");
set("long","这是一条刚刚建造好，还没有粉刷的石路。
");
set("exits",([
"east":__DIR__"1019461907.c","west":__DIR__"1019467371.c",
]));
set("outdoors","[1;35m桃花源[0;0m");
set("group1","taohua");
set("group2","[1;35m桃花源[0;0m");
setup();
setup_var();
}
