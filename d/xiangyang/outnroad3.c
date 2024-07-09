// /d/xiangyang/outnroad3.c

inherit ROOM;

void create()
{
	set("short", "土路");
	set("long", @LONG
你走在鄂陕豫交界的土路上，偶尔会有人骑着马匆匆而过。往西北进入陕
西境内，东北进入河南境内。南边通向襄阳城，西边远处就是老河口了。
LONG
	);
        set("outdoors", "襄阳");

	set("objects", ([
		__DIR__"npc/heiyi-dadao" : 1,
	]) );
	set("exits", ([
		"northwest" : __DIR__"shanxiroad1",
		"northeast" : __DIR__"henanroad1",
		"south" : __DIR__"outnroad2",
		"west" : __DIR__"hanshui1",
	]));

	setup();
}