// road5.c
// Created by Numa 19991027

inherit ROOM;
#include "hmy_def.c"

void create()
{
        set("short", "土路");
	set("long", @LONG
这是一条去黑木崖的必经之路，路上黄沙弥漫。北边不远处有不少行人在
接受日月神教教众的盘查。
LONG
        );
        set("exits", ([ 
          "southeast" : __DIR__"road4",
          "northwest" : __HMY__"shidao",
]));
        set("outdoors", "平定州");
        setup();
}

