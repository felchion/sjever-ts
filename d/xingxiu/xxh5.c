// Room: /d/xingxiu/xxh5.c
// Modify by Lklv 2001.10.16

inherit ROOM;

void create()
{
        set("short", "星宿海");
        set("long", @LONG
这儿便是星宿海的尽头了。举目遥望, 周围是无际的湖泊和沼泽。虽是荒
芜, 然阴暗潮湿, 毒虫繁殖甚富。星宿弟子常来此捕捉毒虫,以备练毒之用。
LONG
        );
	set("outdoors", "星宿");
        set("exits", ([
             "southeast" : __DIR__"xxh3",
             "southwest" : __DIR__"xxh4",
        ]));
        set("objects", ([
                __DIR__"npc/xxdizi.c"  : 1,
        ]));
        setup();
}