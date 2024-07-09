// Write By Yanyang@SJ 2001.10.2
// Room: /d/changan/neijie6.c

inherit ROOM;

void create()
{
        set("short", "内街");
        set("long", @LONG
这是是长安城的居民区，住的都是平常百姓，所以和那些大街比起来，这
里就不那么整洁了。店铺也都是小小门面，比邻而立，但见人来人往、川流不
息，依然一片繁华的景象。
LONG
        );
        set("outdoors", "长安");

        set("exits", ([
                "south"  : __DIR__"neijie7",
                "north"  : __DIR__"neijie5",
        ]));

        set("objects", ([
                __DIR__"npc/fenggou" : 1,
        ]));

        set("incity",1);
	setup();
}