// by xiaoyao

#include <room.h>

inherit ROOM;
void create()
{
	set("short", "古浪");
	set("long", @LONG
古浪城建于西周，本名鬼武，后整修于汉，更名古浪。古浪是西北
地区的毛皮集散地，当初的紫羊羔皮袄，一出于张家口，另一出处就是
古浪了，过了古浪再往西四十里，跨过纪水河即到兰州。
LONG );
	set("exits", ([
		"south"     : __DIR__"yongdeng",
		"east"      : __DIR__"yaocaidian",
		"west"      : __DIR__"xiaojiaqiao",
		"northeast" : __DIR__"dacaigou",
	]));
	set("objects", ([
          "/d/taishan/npc/dao-ke1" : 1,
	]));

	set("outdoors", "黄河");
	set("coor/x",60);
  set("coor/y",90);
   set("coor/z",0);
   setup();
}

