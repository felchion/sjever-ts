// /u/jpei/guiyun/houting.c

inherit ROOM;

void create()
{
	set("short","后厅");
	set("long",@LONG
这是归云庄的后厅。是庄主处理家中私事的地方。东边是一间书房，入口
处放着一扇大屏风；西面则是主人的卧房。
LONG
);
	/*  set("object",([
	__DIR__"obj/luchenfen.c" :     1,
	]));  */
	set("exits",([
		"east" : __DIR__"shufang",
		"west" : __DIR__"wofang",
		"north" : __DIR__"zoulang3",
	]) );
	set("no_clean_up",0);
	set("objects",([
		__DIR__"npc/yahuan" : 1,
	]));  
	setup();
}
