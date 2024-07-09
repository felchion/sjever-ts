// /d/xiangyang/xcx5.c 西城巷
// Lklv 2001.9.25

inherit ROOM;

void create()
{
	set("short", "西城巷");
	set("long", @LONG
这是一条安静的小巷，两边种着许多大树。地面上铺着一层白石子，踩上
去发出沙沙的响声。从这里一直往东走，可以通往朱雀门内街。北面通往白虎
门。巷子西面是一家药材铺，南面是一家私塾，传来一阵阵读书声。
LONG
        );
        set("exits", ([
		"west"  : __DIR__"yaopu",
		"north"  : __DIR__"xcx4",
		"south" : __DIR__"sishu",
		"east" : __DIR__"zqmnj",
	]));

	set("incity",1);
	setup();
}
