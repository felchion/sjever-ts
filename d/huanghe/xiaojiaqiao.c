// by XiaoYao

#include <room.h>

inherit ROOM;
void create()
{
	set("short", "萧家桥");
	set("long", @LONG
在兰州和古浪中间，纪永河水咆哮而过，河水湍激，河床最宽处九
丈四尺，窄处七丈余，度船难过。相传汉年间，本地一位萧姓大财主，
穷六年之功，遍集巧匠，费尽家私，方建成桥，欲到兰州，必先度本桥。
LONG );
	set("exits", ([
		"east"    : __DIR__"gulang",
	]));
	set("no_clean_up", 0);
	set("outdoors", "黄河");
	set("coor/x",50);
  set("coor/y",90);
   set("coor/z",0);
   setup();
}

