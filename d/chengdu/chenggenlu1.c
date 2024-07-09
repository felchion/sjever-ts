// Room: /d/chengdu/chenggenlu1.c
// By Jpei

inherit ROOM;

void create()
{
	set("short", "城跟路");
	set("long", @LONG
这是靠近成都北门城墙跟的一条小路。东边折向东南，西边是北大街。
LONG
	);
        set("outdoors", "成都");

	set("exits", ([
		"east" : __DIR__"chenggenlu2",
		"west" : __DIR__"beidajie4",
	]));

	set("coor/x",-140);
  set("coor/y",10);
   set("coor/z",0);
   setup();
}

