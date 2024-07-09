// /d/xiangyang/djie2.c

inherit ROOM;
#include <ansi.h>

void create()
{
	set("short", "东街");
	set("long", @LONG
这里是军事重镇襄阳城东的一条街道，路面上铺满了青石，两旁是郁郁葱
葱的柳树，树下有几个行人在乘凉。不时地有顶盔贯甲的士兵走过去。向东出
城可以进入安徽、江苏境内。街北边是条巷子口，巷两边也种着许多大树。西
面方向通往城中心。
LONG
	);
        set("outdoors", "襄阳");

	set("exits", ([
		"west" : __DIR__"djie1",
		"east" : __DIR__"eroad1",
                "north" : __DIR__"dcx5",
	]));

	set("incity",1);
	setup();
}
