// automake group room , Sun May 26 22:40:20 2002
inherit ROOM;
#include <ansi.h>
#include "/d/group/gate.h"
void create()
{
set("short","[0;32m武陵[0;0m");
set("long","“武陵山”位于川鄂交界，地势险峻，遍布奇峰、悬崖、峡谷、洞穴。
晋太元中，有渔人偶遇桃花源于此，太守刮刮卡即遣人随其往，寻向
所志，遂迷，不复得路。武当万春流，高尚士也，闻之，欣然规往，
末果，寻病终。后遂无问津者。
[0;0m");
set("exits",([
"up":__DIR__"1019055624.c","northeast":__DIR__"1019461907.c","south":"/d/group/entry/yzeroad1.c",
]));
set("objects",([
"/d/group/obj/biaozhi.c":1,"/d/group/obj/door.c":1,
]));
set("indoors","[1;35m桃花源[0;0m");
set("group1","taohua");
set("group2","[1;35m桃花源[0;0m");
setup();
setup_var();
}
