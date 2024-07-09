// Room: /d/chengdu/chenggenlu3.c
// By Jpei

inherit ROOM;

void create()
{
	set("short", "城跟路");
	set("long", @LONG
这是成都东北城墙跟的一条小路。小路在西北边不远处折而向西，在东南
边不远处折而向南。
LONG
	);
        set("outdoors", "成都");

	set("exits", ([
		"northwest" : __DIR__"chenggenlu2",
		"southeast" : __DIR__"chenggenlu4",
	]));

	set("coor/x",-120);
  set("coor/y",0);
   set("coor/z",0);
   setup();
}

