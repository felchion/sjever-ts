// Room: /u/jpei/thd/road.c

inherit ROOM;

void create()
{
	set("short", "土路");
	set("long", @LONG
一条郊外的土路，远处似乎有座村落。
LONG
	);
	set("outdoors","牛家村");

	set("exits", ([
		"northwest": "/d/hz/qsddao3.c",
		"south": "/d/group/entry/thdroad1",
	]) );

	setup();
}
