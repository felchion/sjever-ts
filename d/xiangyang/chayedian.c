// Room: /d/xianyang/chayedian.c 茶叶店
// Lklv 2001.9.26

inherit ROOM;
void create()
{
        set("short", "茶叶店");
        set("long", @LONG
这是一家生意兴隆的茶叶店，人们进进出出地很是热闹，柜台外面摆着好
几个大个的簸箕，里面平铺着一层茶叶在晾晒。柜台上面则是几个黑色的木头
匣子，里面飘出一股淡淡的茶叶清香。
LONG
        );

        set("exits", ([
                "east" : __DIR__"xcx1",
        ]));

        set("objects", ([
//                __DIR__"npc/laoban" : 1,
        ]));

        set("incity",1);
	setup();
}