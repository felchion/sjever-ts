// Room: /d/xianyang/yaopu.c 药材铺
// Lklv 2001.9.26

inherit ROOM;
void create()
{
        set("short", "药材铺");
        set("long", @LONG
这是一家历史悠久的药材铺，从前的老板因为经营不擅，加上做生意又赔
了钱，所以卖给了别人经营。买下药铺的人说起来也是赫赫有名，他就是江湖
上人称“杀人名医”的平一指。
LONG
        );

        set("no_fight",1);
        set("exits", ([
                "east" : __DIR__"xcx5",
        ]));

        set("objects", ([
		__DIR__"npc/ping" : 1,
        ]));

        set("incity",1);
	setup();
}