// /u/jpei/guiyun/zoulang3.c

inherit ROOM;

void create()
{
	set("short","走廊");
	set("long",@LONG
这是连接中厅与后厅的走廊。走廊曲曲折折，两旁种满了花树。
LONG
);
	/*  set("object",([
	__DIR__"obj/luchenfen.c" :     1,
	]));  */
	set("exits",([
		"east" : __DIR__"zoulang2",
		"south" : __DIR__"houting",
        "north" : __DIR__"wofang1",
	]) );
	set("no_clean_up",0);
	set("outdoors", "归云庄");
	setup();
}
