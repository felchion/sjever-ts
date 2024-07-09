// /d/xiangyang/xjie.c 西街
// Lklv 2001.9.24

inherit ROOM;
void create()
{
        set("short", "西街");
        set("long", @LONG
这是襄阳城西门内的一条街道，路上的行人很多。举目望去，两旁的建筑
似乎都很低矮凌乱，不成章法。行近了看去，却仿佛一个迷阵般格局别致，错
落有序。走在这条街上，耳旁总会不时传来一阵阵喧闹嘈杂的人声。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "west" : __DIR__"bhmnj",
                "east" : __DIR__"cross1",
        ]));

        set("incity",1);
	setup();
}