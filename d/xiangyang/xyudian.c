// /d/xiangyang/xyudian.c 鲜鱼店
// Lklv 2001.9.24

inherit ROOM;
void create()
{
        set("short", "鲜鱼店");
        set("long", @LONG
这是一家卖鲜鱼的铺子，店里到处摆放着大大小小的鱼篓，水淋淋的。靠
门边放着一口大水缸，里面有很多活蹦乱跳的鲜鱼。一个身上扎围裙的中年女
子，站在那里，眼光望向屋外。
LONG
        );

        set("exits", ([
                "east" : __DIR__"bcx3",
        ]));

        set("objects", ([
		__DIR__"npc/liersao" : 1,
        ]));
        set("incity",1);
	setup();
}