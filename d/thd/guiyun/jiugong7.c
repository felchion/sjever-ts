// Room: /u/jpei/guiyun/jiugong7.c

#include <ansi.h>

inherit ROOM;

#define HERENO	7

int do_get(string arg);
int do_drop(string arg);
int do_look(string arg);
int do_inv(string arg);

void create()
{
	set("short", "九宫桃花阵");
	set("outdoors","归云庄");

	set("exits", ([
		"east" : __DIR__"jiugong8",
		"north" : __DIR__"jiugong4",
	]));
	setup();
}

#include "jiugong.c"