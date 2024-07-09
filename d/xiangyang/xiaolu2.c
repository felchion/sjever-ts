// /d/xiangyang/xiaolu2.c
// Lklv Modify 2001.9.22

inherit ROOM;
void create()
{
	set("short", "小路");
	set("long", @LONG
你来到了鄂川交界的小路上，偶尔会有人骑着马匆匆而过，卷起飞扬的尘
土。东北边通向襄阳城，往西则进入四川的大道。
LONG
	);
        set("outdoors", "襄阳");

	set("exits", ([
		"northeast" : __DIR__"xiaolu1",
		"west" : "/d/group/entry/cderoad4",
	]));

	setup();
}