// Room: /d/xianyang/sishu.c 私塾
// Lklv 2001.9.22

inherit ROOM;
void create()
{
        set("short", "私塾");
        set("long", @LONG
这是一间新开张的私塾，门上的对联看起来贴上去还没有多少日子。一位
年轻的私塾先生，在地中央来回踱着步子。他原本是个秀才，因为上次秋试落
弟，生活又没有着落，没办法才开始教书。
LONG
        );

        set("exits", ([
                "north" : __DIR__"xcx5",
        ]));

        set("objects", ([
		__DIR__"npc/xiansheng" : 1,
        ]));

        set("incity",1);
	setup();
}