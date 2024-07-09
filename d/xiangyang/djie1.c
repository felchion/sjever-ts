// /d/xiangyang/djie1.c

inherit ROOM;
#include <ansi.h>

void create()
{
	set("short", "东街");
	set("long", @LONG
这里是军事重镇襄阳城东的一条街道，路面上铺满了青石，两旁是郁郁葱
葱的柳树，树下有几个行人在乘凉。不时地有顶盔贯甲的士兵走过去。向东出
城可以进入安徽、江苏境内。街北边是家当铺，门口坐着几个穷汉。
LONG
	);
        set("outdoors", "襄阳");

	set("exits", ([
		"west" : __DIR__"cross2",
		"east" : __DIR__"djie2",
                "north" : __DIR__"dangpu",
	]));

	set("incity",1);
	setup();
}