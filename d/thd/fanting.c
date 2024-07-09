// Room: /u/jpei/thd/fanting.c

inherit ROOM;

void create()
{
	set("short", "饭厅");
	set("long", @LONG
这是黄药师一家人吃饭的地方。
LONG
	);
	set("no_clean_up", 0);
	set("resource/water", 1);

	set("exits", ([
		"east" : __DIR__"neishi1.c",
		"north" : __DIR__"chufang.c",
	]));

	setup();
}
