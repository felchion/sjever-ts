// /d/xiangyang/njie2.c 南街
// Lklv 2001.9.24

inherit ROOM;
void create()
{
        set("short", "南街");
        set("long", @LONG
这是由襄阳城中心通往南门的一条大街，路面很宽，两旁种满了郁郁葱葱
的垂柳。地面上铺着大块的青石，平坦整洁。向南望去，隐约可以看见城门上
的城楼和女儿墙。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "south" : __DIR__"njie3",
                "north" : __DIR__"njie1",
        ]));

        set("objects", ([
		__DIR__"npc/wanyanping" : 1,
        ]));

        set("incity",1);
	setup();
}