// /d/xiangyang/ncx5.c 南城巷
// Lklv 2001.9.25

inherit ROOM;

void create()
{
	set("short", "南城巷");
	set("long", @LONG
这是一条安静的小巷，两边种着许多大树。从这里一直往东走，可以通往
青龙门内街。巷子东面是一家打铁铺，两扇大门敞开着。里面传出叮叮当当的
打铁声。
LONG
        );
        set("exits", ([
		"east"  : __DIR__"datiepu",
		"south"  : __DIR__"ncx4",
		"north"  : __DIR__"eroad1",
	]));

	set("incity",1);
	setup();
}
