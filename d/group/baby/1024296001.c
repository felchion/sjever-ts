// automake group room , Tue Jun 18 02:45:32 2002
inherit ROOM;
#include <ansi.h>
#include "/d/group/gate.h"
void create()
{
set("short","[1;37m稻[1;35m香[1;37m村[0;0m");
set("long","转过山怀中，隐隐露出一带[0;33m黄泥[0;0m筑就矮墙，墙头皆用稻茎掩护。有几百株
[1;31m杏花[0;0m，如喷火蒸霞一般。里面数楹茅屋。外面却是桑、榆、槿、柘，各色树稚
新条，随其曲折，编就两溜[1;36m青篱[0;0m。篱外山坡之下，有一土井，旁有桔槔辘轳之
属。下面分畦列亩，佳蔬[1;32m菜花[0;0m，漫然无际。
[0;0m");
set("exits",([
"up":__DIR__"1024298179.c","south":__DIR__"1024326114.c","north":__DIR__"1024326043.c","down":__DIR__"1024295444.c","east":__DIR__"1024325985.c","west":__DIR__"1024326083.c",
]));
set("outdoors","[1;37m凝[1;35m香[1;37m阁[0;0m");
set("group1","baby");
set("group2","[1;37m凝[1;35m香[1;37m阁[0;0m");
setup();
setup_var();
}
