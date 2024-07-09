// /d/xiangyang/outnroad4.c

inherit ROOM;

void create()
{
	set("short", "土路");
	set("long", @LONG
你走在陕鄂交界的土路上，偶尔会有人骑着马匆匆而过。这里已经是陕西
地界，往南进入湖北境内。西边是丝绸古路，西北有一个小村落。
LONG
	);
        set("outdoors", "襄阳");

	set("exits", ([
		"west" : "/d/xingxiu/xxroad1",
		"south" : __DIR__"outnroad3",
		"northwest" : "/d/village/hsroad3",
	]));

	setup();
}

