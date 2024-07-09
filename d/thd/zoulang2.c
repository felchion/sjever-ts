// Room: /u/jpei/thd/zoulang2.c

inherit ROOM;

void create()
{
	set("short", "回廊");
	set("long", @LONG
一条连接大厅和内室的回廊。
LONG
	);
	set("no_clean_up", 0);
	set("outdoors","桃花岛");

	set("exits", ([
		"north": __DIR__"neishi1.c",
		"south": __DIR__"zoulang1.c",
	]) );

	setup();
}

