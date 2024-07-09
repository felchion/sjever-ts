// automake group room , Mon Jun 17 23:06:48 2002
inherit ROOM;
#include <ansi.h>
#include "/d/group/gate.h"
void create()
{
set("short","房间");
set("long","这是一座刚刚建造好，还没有粉刷的房屋。
");
set("exits",([
"down":__DIR__"1024326043.c",
]));
set("indoors","[1;37m凝[1;35m香[1;37m阁[0;0m");
set("group1","baby");
set("group2","[1;37m凝[1;35m香[1;37m阁[0;0m");
setup();
setup_var();
}
