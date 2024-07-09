// Room: tulu2.c 土路
// By jpei

inherit ROOM;

void create()
{
	set("short", "土路");
	set("long", @LONG
这是一条尘土飞扬的小路，因为四川自古就是富庶之地，所以放眼望去，
四周的田垅里郁郁青青。
LONG);
	set("outdoors", "峨嵋");

	set("exits", ([
		"northeast" : __DIR__"tulu3",
		"southwest" : __DIR__"tulu1",
	]));

	set("coor/x",-170);
  set("coor/y",-90);
   set("coor/z",0);
   setup(); 
}
