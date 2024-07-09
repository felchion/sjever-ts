// /d/xiangyang/bhmnj.c 白虎门内街
// Lklv Modify 2001.9.22

inherit ROOM;
void create()
{
        set("short", "白虎门内街");
        set("long", @LONG
这里是襄阳城的西门——白虎门内的一条街道。举目望去，道路两旁的建
筑都很低矮凌乱，不成章法。但是却很热闹。走在这条小街上，耳旁总会不时
传来一阵阵喧闹嘈杂的人声。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "west" : __DIR__"baihumen",
                "east" : __DIR__"xjie",
                "north" : __DIR__"bcx4",
                "south" : __DIR__"xcx1",
        ]));

        set("objects", ([
		NPC_D("guanfu/bing") : 2,
        ]));
        set("incity",1);
	setup();
}
