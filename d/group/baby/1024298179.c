// automake group room , Mon Jun 17 23:51:31 2002
inherit ROOM;
#include <ansi.h>
#include "/d/group/gate.h"
void create()
{
set("short","[1;37m凝[1;35m香[1;37m闰[0;0m");
set("long","[1;37m刚至房门，便有一股细细的甜香袭人而来，但见珠帘绣幕，画栋雕檐，说
不尽那[1;33m光摇朱户金铺地[1;37m，[1;36m雪照琼窗玉作宫[1;37m，更见[1;35m仙花馥郁[1;37m，异草芬芳，真好个
所在。向壁上看时，有唐伯虎画的「[1;32m海棠春睡图[1;37m」，两边有宋学士秦太虚写的
一副对联，其联云：

　　　　　[1;35m嫩寒锁梦因春冷              芳气笼人是酒香[0;0m

");
set("exits",([
"down":__DIR__"1024296001.c",
]));
set("objects",([
"/d/group/obj/qiangui.c":1,
]));
set("indoors","[1;37m凝[1;35m香[1;37m阁[0;0m");
set("group1","baby");
set("group2","[1;37m凝[1;35m香[1;37m阁[0;0m");
setup();
setup_var();
}
