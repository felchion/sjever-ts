// automake group room , Mon Jun 17 14:58:34 2002
inherit ROOM;
#include <ansi.h>
#include "/d/group/gate.h"
void create()
{
set("short","[1;37m凝[1;35m香[1;37m幼稚园[0;0m");
set("long","这是一所[1;37m囡囡[0;0m创办的[1;35m幼稚园[0;0m，小朋友们要入学，请先填表登记，并交纳报
名费一千锭[1;33m黄金[0;0m或者一百张[1;32m壹仟两银票[0;0m，联系人：[1;37mbaby[0;0m
    附注：如报名不获批准，报名费恕不退还。
[0;0m");
set("exits",([
"southeast":"/d/group/entry/hhshulin5.c","west":__DIR__"1024295444.c",
]));
set("objects",([
"/d/group/obj/biaozhi.c":1,"/d/group/obj/door.c":1,
]));
set("indoors","[1;37m凝[1;35m香[1;37m阁[0;0m");
set("group1","baby");
set("group2","[1;37m凝[1;35m香[1;37m阁[0;0m");
setup();
setup_var();
}
