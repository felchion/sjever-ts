// Room: /u/jpei/thd/road2.c

inherit ROOM;

void create()
{
	set("short", "土路");
	set("long", @LONG
一条郊外的土路，东南方有着一座小小的村落。
LONG
	);
	set("outdoors","牛家村");

	set("exits", ([
		"northwest": "/d/group/entry/thdroad1",
		"southeast": __DIR__"njwest.c",
	]) );

	setup();
}
