// /d/xiangyang/outeroad1.c

inherit ROOM;
void create()
{
	set("short", "东门外");
	set("long", @LONG
这是襄阳东门外的一条青石大道，人来人往非常繁忙，不时有人骑着马匆
匆而过。大道往东进入安徽境内。西边通向襄阳城东面，青龙门。
LONG
	);
	set("outdoors", "襄阳");
	set("objects", ([
		__DIR__"npc/duxingxia" : 1,
	]) );
	set("exits", ([
		"east"  : "/d/city/wroad3",
		"west" : __DIR__"qinglongmen",
	]));
	setup();
}
