// Room: d/xiangyang/shuoshug.c 说书馆
// Lklv 2001.9.25

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", HIC"说书馆"NOR);
        set("long", @LONG
这是一家说书馆，地方不大，地中央几张放有两排长条桌，桌周围摆了几
条长条凳子。北首放了一张八仙桌，墙上挂有一副对联，对联上的字写得龙飞
凤舞。
LONG
        );

        set("exits", ([
                "south" : __DIR__"ncx1",
        ]));

        set("objects", ([
                __DIR__"npc/shuren" : 1,
        ]));
        set("incity",1);
	setup();
}
