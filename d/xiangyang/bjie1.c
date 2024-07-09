// /d/xiangyang/bjie1.c 北街
// Lklv 2001.9.24

inherit ROOM;
void create()
{
        set("short", "北街");
        set("long", @LONG
这是由襄阳北门通往城中心的街道，路面很宽，两旁种着柳树。街道上人
来人往，很是热闹。经常有衣帽光鲜的少年，骑马向北门而去。东面是一家生
意惨淡的赌场，门口挂有一张旗，上面画个硕大的骰子。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "south" : __DIR__"bjie2",
                "north" : __DIR__"nroad1",
                "west" : __DIR__"bcx1",
                "east" : __DIR__"duchang",
        ]));

        set("incity",1);
	setup();
}