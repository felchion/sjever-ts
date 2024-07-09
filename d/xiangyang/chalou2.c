// Room: d/fuzhou/chalou2.c
// Lklv 2001.9.10

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", HIC"茶楼二楼"NOR);
        set("long", @LONG
这是茶楼的二楼，屋内摆着几排长条桌，旁边凳子上坐了几位茶客，或高
声谈笑，或交头接耳，来自各地的口音混杂在一起，听起来很是热闹。室内飘
散着一股淡淡的茶香。
LONG
        );
        set("resource/water", 1);

        set("exits", ([
                "down" : __DIR__"chalou",
        ]));
        set("objects", ([
                __DIR__"npc/gongzi" : 1,
                __DIR__"npc/haoke" : 1,
        ]));
        set("incity",1);
	setup();
}
