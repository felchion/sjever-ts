// Room: lianwu.c

inherit ROOM;

void create()
{
	set("short", "练武场");
	set("long", @LONG
你走进一座三开间的大厅，打通了成为一个大场子。场子两边的兵器架上摆满了
各种兵刃。
LONG
	);
 

	set("objects", ([
//		__DIR__"npc/": 1,
	]));
	
	set("exits", ([
		"east":__DIR__"tianjing",
	]) );

	setup();
}
