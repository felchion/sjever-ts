// /d/xiangyang/ncx3.c 南城巷
// Lklv 2001.9.25

inherit ROOM;

void create()
{
	set("short", "南城巷");
	set("long", @LONG
这是一条安静的小巷，两边种着许多大树。从这里一直往东走，可以通往
青龙门内街。巷子南面是一家大宅院，两扇大门敞开着。大门旁边站有两个家
丁。路北面是个兵营，一些兵丁进进出出。
LONG
        );
        set("exits", ([
		"east"  : __DIR__"ncx4",
		"west"  : __DIR__"ncx2",
		"north"  : __DIR__"bingying",
		"south" : __DIR__"wen/damen",
	]));

	set("incity",1);
	setup();
}
