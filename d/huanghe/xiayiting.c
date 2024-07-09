// by XiaoYao

#include <room.h>

inherit ROOM;
void create()
{
	set("short", "侠义厅");
	set("long", @LONG
这是黄河帮议事的大厅，厅上挂着「侠义厅」三字匾额。厅里靠
北朝南放着一张虎皮交椅，两边摆设着几副红木桌椅，非常有气派。
墙上附庸风雅地挂了几幅丹青书法。
LONG );
	set("exits", ([
		"east"  : __DIR__"guangchang",
	]));
	set("objects", ([
		__DIR__"npc/sha" : 1,
		__DIR__"npc/peng" : 1,
		__DIR__"npc/liang" : 1,
	]));

	set("outdoors", "黄河");
	set("coor/x",90);
  set("coor/y",100);
   set("coor/z",0);
   setup();
}

