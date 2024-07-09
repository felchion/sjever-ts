// by XiaoYao

#include <room.h>

inherit ROOM;

void create()
{
	set("short", "土门子");
	set("long", @LONG
土门子也是以前驻军的地方，周围全是沙漠，中间一块小小的绿洲，
本来这里有当地的牧民居住，可是前不就来了一伙马贼占据了这里以这
里为大本营四出劫掠，无恶不做。
LONG );
	set("exits", ([
		"south"     : __DIR__"wuwei",
		"northeast" : __DIR__"shixiazi",
          "northwest" : "/d/hengshan/jinlongxia",
//       "east" : "/d/hmy/pingding/road1",
	]));
	set("no_clean_up", 0);
	set("outdoors", "黄河");
	set("coor/x",60);
  set("coor/y",130);
   set("coor/z",0);
   setup();
}

int valid_leave(object me, string dir)
{
        if (userp(me) && !wizardp(me) && !me->query_temp("hmy_tester") && dir == "east")  
                return notify_fail("只有测试者才能够通过。\n");
        return ::valid_leave(me, dir);
}
