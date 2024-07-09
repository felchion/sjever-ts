// /d/xiangyang/outwroad1.c

inherit ROOM;

void create()
{
	set("short", "山间小路");
	set("long", @LONG
你走在一条蜿蜒而上的山间小路上，这里十分荒凉，前面不远处就是
一些小树林，南面有条小路通向树林深处。
LONG
	);
        set("outdoors", "襄阳");

	set("exits", ([
		"northdown" : __DIR__"shanlu1",
		"southwest" : __DIR__"shanlu3",
	]));

	setup();
}

