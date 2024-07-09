// Room: /d/xianyang/shaobingp.c 烧饼铺
// Lklv 2001.9.26

inherit ROOM;
void create()
{
        set("short", "烧饼铺");
        set("long", @LONG
这是一家烧饼铺，墙壁的颜色很黑，铺子里的火炉上有口大锅，锅里整齐
的摆了一圈烙好的烧饼，散发出一股特有的烧饼香味。这家铺子的烧饼在附近
很有名，不时有人进屋来买上一个两个烧饼。
LONG
        );

        set("exits", ([
                "east" : __DIR__"xcx3",
        ]));

        set("objects", ([
		__DIR__"npc/shaobingliu" : 1,
        ]));

        set("incity",1);
	setup();
}