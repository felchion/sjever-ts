//caodi.c 草地 

#include <ansi.h>
#include <room.h>

inherit ROOM;
void create()
{
	set("short",HIG"草地"NOR);
	set("long", @LONG
一片绿幽幽的草坪，不由的想起了，修带不为歌舞缓，浓铺堪作醉人茵，
无意衬香衾。晴霁后，颜色一般新，游子不归生满地，佳人远意正青春，留咏
卒难伸。
LONG
	);

	set("exits", ([
		"west" : __DIR__"huilang4",
		"southeast" : __DIR__"shulin10",
		"northeast" : __DIR__"shulin12",
		"east" : __DIR__"shulin11",
	]));
        
	set("outdoors", "试剑山庄");
	setup();
}

#include "sjsz_room.h";
