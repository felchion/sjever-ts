// by xiaoyao

#include <room.h>

inherit ROOM;
void create()
{
	set("short", "广场");
	set("long", @LONG
广场占地甚大，部置严谨。往西过去可达黄河帮的「侠义厅」。
周围则是些房舍，供帮众居住。
LONG );
	set("exits", ([
		"east"  : __DIR__"huanghegate",
		"west"  : __DIR__"xiayiting",
	]));
	set("objects", ([
		__DIR__"npc/wu" : 1,
		__DIR__"npc/qian" : 1,
	]));

	set("outdoors", "黄河");
	set("coor/x",100);
  set("coor/y",100);
   set("coor/z",0);
   setup();
}

