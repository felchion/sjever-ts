// /d/xiangyang/ncx4.c 南城巷
// Lklv 2001.9.25

inherit ROOM;

void create()
{
	set("short", "南城巷");
	set("long", @LONG
这是一条安静的小巷，两边种着许多大树。从这里一直往东走，可以通往
青龙门内街。巷子东面是一家木匠铺，门板漆黑，窗户上镂着窗花。北面就是
不远处就是青龙门了。
LONG
        );
        set("exits", ([
		"east"  : __DIR__"mujiangpu",
		"west"  : __DIR__"ncx3",
		"north"  : __DIR__"ncx5",
	]));

	set("incity",1);
	setup();
}
