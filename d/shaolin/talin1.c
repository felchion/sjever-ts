// Room: /d/shaolin/talin1.c
// Date: hunthu

#include <room.h>
inherit ROOM;


void create()
{
	set("short", "塔林");
	set("long", @LONG
这里是少林历代僧侣的墓地，上百座墓塔式样繁多，造型各异。你置身
其中，只看得眼花缭乱，不知该往哪个方向走，你似乎迷路了。
LONG
	);

	set("exits", ([
		"northeast" : __DIR__"talin2",
		"southeast" : __DIR__"talin"+(random(10)+1),
		"northwest" : __DIR__"talin"+(random(10)+1),
		"southwest" : __DIR__"talin"+(random(10)+1),
		"south"     : __DIR__"talin"+(random(10)+1), 
                "west"      : __DIR__"talin"+(random(10)+1),
                "north"     : __DIR__"talin"+(random(10)+1),
                "east" : __DIR__"slyuan",
	]));

	create_door("east", "木门", "west", DOOR_CLOSED);

	set("outdoors", "shaolin");

	set("coor/x",10);
  set("coor/y",310);
   set("coor/z",120);
   setup();

}

