// Room: /d/xiangyang/bingying.c
// Lklv 2001.9.25

inherit ROOM;

void create()
{
        set("short", "兵营");
        set("long", @LONG
这里是一个兵营，因为要防御蒙古铁骑的进攻，所以城里的兵大多都开到
城外和城墙上守城去了，这里只有几个留守的官兵。两个武将面有忧色地凑在
一起在谈论着什么。
LONG
        );
        set("exits", ([
                "south" : __DIR__"ncx3",
        ]));

        set("objects", ([
                NPC_D("guanfu/wujiang") : 2,
                NPC_D("guanfu/bing") : 4,
	]));
	set("incity",1);
	setup();
}
