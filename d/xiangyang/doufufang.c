// Room: /d/xianyang/doufufang.c 豆腐坊
// Lklv 2001.9.26

inherit ROOM;
void create()
{
        set("short", "豆腐坊");
        set("long", @LONG
这是一家豆腐坊，门口的树上栓了一头毛驴，地中间有一盘很大很平整的
石磨。墙角有好几个大缸，缸上面漂浮着一层薄薄的豆皮。西首的地上堆着一
堆麻袋，有个开口的露出了里面的黄豆。屋子里弥漫着一股豆花的味道。
LONG
        );

        set("exits", ([
                "east" : __DIR__"xcx2",
        ]));

        set("objects", ([
//                __DIR__"npc/laoban" : 1,
        ]));

        set("incity",1);
	setup();
}