// by xiaoyao

#include <room.h>

inherit ROOM;

void create()
{
	set("short", "打柴沟");
	set("long", @LONG
打柴沟地势不如乌鞘岭那样险要，比较平坦，官道两边全是密林，
时常有樵夫砍柴的声音从林中传出。
LONG );
	set("exits", ([
		"southwest" : __DIR__"gulang",
		"north"     : __DIR__"wuqiao",
	]));
	set("objects", ([
		__DIR__"npc/qiaofu" : 1,
	]));

	set("outdoors", "黄河");
	set("coor/x",70);
  set("coor/y",100);
   set("coor/z",0);
   setup();
}

