// kefang.c

inherit ROOM;
#include "hmy_def.c"

void create()
{
        set("short", "客房");
        set("long", @LONG
这是一间已经被人包下的客房，里面有一名看起来似乎心事重重的女子坐
在火盆旁，火盆里的熊熊火光映在她的脸上，更增娇艳。
LONG
        );

//	set("no_sleep_room",1);
//	set("no_fight",1);
//	set("no_get",1);
        set("exits", ([
                "south" : __DIR__"kedian",
        ]));
        
/*
        set("objects", ([
                __HMY__"npc/yingying" : 1,
        ]));
*/
        setup();
}
