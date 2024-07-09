// /d/xiangyang/shanlu3.c
// Lklv Modify 2001.9.22

inherit ROOM;
void create()
{
	set("short", "山间小路");
	set("long", @LONG
你进入了一片小森林，走在林间小路上，密密麻麻的树林挡住了阳光，
这里显得十分阴暗，树木交错纵横，这里没有什么出路了.
LONG
	);
        set("outdoors", "襄阳");

	set("exits", ([
		"northeast" : __DIR__"shanlu2",
	]));

	set("objects",([
		__DIR__"npc/dingpopo" : 1,
		__DIR__"npc/pingpopo" : 1,
		__DIR__"npc/ruipopo" : 1,

	]));

	setup();
}