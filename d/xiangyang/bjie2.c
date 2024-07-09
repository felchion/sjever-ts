// /d/xiangyang/bjie2.c 北街
// Lklv 2001.9.24

inherit ROOM;
void create()
{
        set("short", "北街");
        set("long", @LONG
这是由襄阳北门通往城中心的街道，路面很宽，两旁种着柳树。街道上人
来人往，很是热闹。东面是一家老字号的茶楼，门口高高飘着对幌子，楼上人
声嘈杂，看起来似乎生意不错。几个江湖打扮的汉子正行上楼去。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "south" : __DIR__"bjie3",
                "north" : __DIR__"bjie1",
                "east" : __DIR__"chalou",
        ]));
        set("objects", ([
		CLASS_D("gaibang/yelvqi") : 1,
        ]));
        set("incity",1);
	setup();
}