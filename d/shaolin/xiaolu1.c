// Room: /d/shaolin/xiaolu1.c
// Date: YZC 96/01/19
// by bren

inherit ROOM;

void create()
{
	set("short", "林中小路");
	set("long", @LONG
这是一片密密的竹林。这里人迹罕至，惟闻足底叩击路面，有僧敲木鱼
声；微风吹拂竹叶，又如簌簌禅唱。令人尘心为之一涤，真是绝佳的禅修所
在。
LONG
	);

	set("exits", ([
		"east" : __DIR__"xiaolu2",
		"west" : __DIR__"sblu-2",
	]));

	set("outdoors", "shaolin");
	set("coor/x",60);
  set("coor/y",370);
   set("coor/z",130);
   setup();
}



