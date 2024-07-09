// automake group room , Wed Jul 10 14:36:50 2002
inherit ROOM;
#include <ansi.h>
#include "/d/group/gate.h"
void create()
{
set("short","石路");
set("long","这是一条刚刚建造好，还没有粉刷的石路。
");
set("exits",([
"east":__DIR__"1019467295.c","down":__DIR__"1025842584.c",
]));
set("objects",([
__DIR__"npc/1025050720.c":1,__DIR__"npc/1026282922.c":1,
]));
set("outdoors","[1;35m桃花源[0;0m");
set("group1","taohua");
set("group2","[1;35m桃花源[0;0m");
setup();
setup_var();
}
