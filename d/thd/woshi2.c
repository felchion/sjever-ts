// Room: /u/jpei/thd/woshi2.c

inherit ROOM;

void create()
{
	set("short", "卧室");
	set("long", @LONG
这是一间布置得很整齐的卧室。
LONG
	);
	set("no_clean_up", 0);

	set("exits", ([
		"west" : __DIR__"neishi2.c",
	]));

	setup();
}


