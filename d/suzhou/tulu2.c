// tulu2.c
// By Jpei

inherit ROOM;

void create()
{
        set("short", "土路");
        set("long",@long
你来到了一条土路上。西南边有条江，远远看去不甚宽阔。往东北边走可
以找到是去苏州的官道。
long);
	set("outdoors", "suzhou");
	set("exits",([
		"northeast" : __DIR__"tulu1",
		"southwest" : __DIR__"jiangbian",
	]));
	set("coor/x",50);
  set("coor/y",-230);
   set("coor/z",0);
   setup();
}
