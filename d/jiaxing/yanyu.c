// Room: /d/jiaxing/yanyu.c

// by shang 97/6
inherit ROOM;

void create()
{
	set("short", "烟雨楼");
	set("long", @LONG
烟雨楼建在南湖中心的一个小岛上。南湖终年不断的水气把它
笼罩得迷迷蒙蒙的。
LONG
	);

	set("exits", ([
                "north" : __DIR__ "nanhu1",
	]));

	set("coor/x",160);
  set("coor/y",-320);
   set("coor/z",0);
   setup();
}

