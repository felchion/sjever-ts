// Room: /d/shaolin/bamboo4.c
// Date: YZC 96/01/19

inherit ROOM;




void create()
{
	set("short", "竹林");
	set("long", @LONG
这是一片密密的竹林。这里人迹罕至，惟闻足底叩击路面，有僧敲木鱼
声；微风吹拂竹叶，又如簌簌禅唱。令人尘心为之一涤，真是绝佳的禅修所
在。
LONG
	);

	set("exits", ([
		"north" : __DIR__"bamboo"+(random(13)+1),
		"south" : __DIR__"bamboo5",
		"east" : __DIR__"bamboo"+(random(13)+1),
		"west" : __DIR__"bamboo"+(random(13)+1),
	]));

	set("outdoors", "shaolin");
	set("coor/x",60);
  set("coor/y",530);
   set("coor/z",150);
   setup();
}
/*
int valid_leave(object me, string dir)
{

	if (dir == "south" ) me->add_temp("bamboo/count", 1);
	else 			 me->add_temp("bamboo/count", -1);

	return ::valid_leave(me, dir);
}
*/

