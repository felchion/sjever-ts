// Room: /d/chengdu/chenggenlu2.c
// By Jpei

inherit ROOM;

void create()
{
	set("short", "城跟路");
	set("long", @LONG
这是靠近成都北门城墙跟的一条小路。西边不远处就是北大街。
LONG
	);
        set("outdoors", "成都");

	set("exits", ([
		"west" : __DIR__"chenggenlu1",
		"southeast" : __DIR__"chenggenlu3",
	]));

	set("coor/x",-130);
  set("coor/y",10);
   set("coor/z",0);
   setup();
}

