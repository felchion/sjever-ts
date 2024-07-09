// /d/xiangyang/outsroad1.c

inherit ROOM;
void create()
{
	set("short", "南门外");
	set("long", @LONG
这是襄阳南门外的一条青石大道，路上人来人往非常繁忙，不时有人骑着
马匆匆而过。这里往南进入湖南境内。北边通向襄阳城的南门朱雀门。
LONG
	);
        set("outdoors", "襄阳");

	set("exits", ([
		"south" : __DIR__"outsroad2",
		"north" : __DIR__"zhuquemen",
	]));
        set("objects", ([
                NPC_D("poorman") : 1,
        ]));
	setup();
}