// Room: /d/emei/qingshijie.c

inherit ROOM;

void create()
{
	set("short", "青石阶");
	set("long", @LONG
已经是峨嵋山脚下了。南面一条通往峨嵋的青石阶，两旁绿树荫蔽，空气
十分清新。西南方通向南诏的大理国。西边好象有人搭了个小草棚。
LONG
	);
	set("outdoors", "峨眉山");
	set("no_clean_up", 0);
	set("exits", ([ 
	  "southup" : __DIR__"baoguosm.c",
           "west" : __DIR__"caopeng",
          "east" : "/d/chengdu/tulu1",
]));

	set("coor/x",-180);
  set("coor/y",-110);
   set("coor/z",0);
   setup();
}
