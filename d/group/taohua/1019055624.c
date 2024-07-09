// automake group room , Sun May 26 14:38:23 2002
inherit ROOM;
#include <ansi.h>
#include "/d/group/gate.h"
void create()
{
set("short","房间");
set("long","这是一座刚刚建造好，还没有粉刷的房屋。
");
set("exits",([
"down":__DIR__"1019053619.c",
]));
set("objects",([
"/d/group/obj/qiangui.c":1,
]));
set("indoors","[1;35m桃花源[0;0m");
set("group1","taohua");
set("group2","[1;35m桃花源[0;0m");
setup();
setup_var();
}
