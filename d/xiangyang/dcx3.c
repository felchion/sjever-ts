// /d/xiangyang/dcx3.c 东城巷
// Lklv 2001.9.25

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", "东城巷");
        set("long", @LONG
这是由青龙门通往玄武门的一条小巷，地处襄阳城东。路面上铺满了大大
小小的青石，两旁是郁郁葱葱的柳树，树下有几个行人在乘凉。远处的巷口有
几个穷汉在躺着晒太阳，路东有家成衣铺，半开着门。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "north" : __DIR__"dcx2",
                "south" : __DIR__"dcx4",
                "east" : __DIR__"chengyipu",
        ]));

        set("objects", ([
		__DIR__"npc/guofu" : 1,
        ]));

        set("incity",1);
	setup();
}