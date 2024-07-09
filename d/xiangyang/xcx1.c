// /d/xiangyang/xcx1.c 西城巷
// Lklv 2001.9.25

inherit ROOM;

void create()
{
	set("short", "西城巷");
	set("long", @LONG
这是一条安静的小巷，两边种着许多大树。地面上铺着一层白石子，踩上
去发出沙沙的响声。从这里一直往南走，可以通往朱雀门内街。北面通往白虎
门。巷子西面是一家店铺，两扇门敞开着。
LONG
        );
        set("exits", ([
		"west"  : __DIR__"chayedian",
		"north"  : __DIR__"bhmnj",
		"south" : __DIR__"xcx2",
	]));

	set("incity",1);
	setup();
}
