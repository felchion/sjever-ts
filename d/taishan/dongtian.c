// Room: /d/taishan/.c

inherit ROOM;

void create()
{
	set("short", "东天门");
	set("long", @LONG
这里是岱顶的东面出口。往东依次经过伏虎门峡口、鹰愁涧等，便可下山。
LONG
	);

	set("exits", ([
		"west" : __DIR__"tanhai",
	]));

	set("outdoors", "泰山");
	setup();
}

