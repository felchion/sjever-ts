// Room: /d/city/eroad1.c

inherit ROOM;

void create()
{
	set("short", "大驿道");
	set("long", @LONG
你走在一条尘土飞扬的大驿道上。四周人来人往。挑担子的行商，赶着大
车的马夫，熙熙攘攘，非常热闹。不时还有两三骑快马从身边飞驰而过，扬起
一路尘埃。道路两旁是整整齐齐的杨树林。
LONG
	);

	set("exits", ([
		"west" : __DIR__"dongmen",
		"east" : __DIR__"eroad2",
	]));

	set("objects",([
		__DIR__"npc/tiao-fu" : 1,
	]));

 	set("outdoors", "扬州");
	set("coor/x",140);
  set("coor/y",-10);
   set("coor/z",0);
   setup();
}



