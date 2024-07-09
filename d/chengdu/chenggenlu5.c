// Room: /d/chengdu/chenggenlu5.c
// By Jpei

inherit ROOM;

void create()
{
	set("short", "城跟路");
	set("long", @LONG
这是靠近成都东门城墙跟的一条小路。北边折向西北，南边是东大街。
LONG
	);
        set("outdoors", "成都");

	set("exits", ([
		"north" : __DIR__"chenggenlu4",
		"south" : __DIR__"dongdajie4",
	]));

	set("coor/x",-110);
  set("coor/y",-20);
   set("coor/z",0);
   setup();
}

