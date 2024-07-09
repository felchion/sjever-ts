// /d/xiangyang/dcx4.c 东城巷
// Lklv 2001.9.25

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", "东城巷");
        set("long", @LONG
这里是由青龙门通往玄武门的一条小巷，地处襄阳城东，巷子两旁种着低
矮的树墙，间或点缀着一丛丛鲜花。街道上的行人都很悠闲，几个姑娘结伴在
前面边走边说笑着。路东有扇大门敞开着，门边挂着一个牌子(paizi)。
LONG
        );
        set("outdoors", "襄阳");

        set("item_desc", ([
                "paizi" : HIC"东城戏院\n"NOR,
        ]));

        set("exits", ([
                "north" : __DIR__"dcx3",
                "south" : __DIR__"dcx5",
                "east" : __DIR__"xiyuan",
        ]));

        set("objects", ([
		NPC_D("poorman") : 1,
        ]));

        set("incity",1);
	setup();
}