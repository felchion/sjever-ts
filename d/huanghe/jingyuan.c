// by xiaoyao

#include <room.h>
#include <wanted.h>

inherit ROOM;

void create()
{
	set("short", "靖远");
	set("long", @LONG
靖远是个不大不小的城镇，黄沙在离城四十里的地方被一排排的钻天
白杨树给挡住了。因此这里有了一块较大的绿洲，聚集了不少从别处来的
移民。这些白杨据说是当年唐朝李靖赶走了突厥后在这里屯兵时所种。这
也是靖远得名的由来。西边不远就是黄河，南面是一大片沙漠。
LONG );
	set("exits", ([
		"north"     : __DIR__"hongshanxia",
		"south"     : __DIR__"shamo",
	]));

	set("outdoors", "黄河");
	set("coor/x",100);
  set("coor/y",130);
   set("coor/z",0);
   setup();
}

int valid_leave(object me, string dir)
{
	if (dir == "south" && is_wanted(me))
		return 0;
	return ::valid_leave(me, dir);
}
