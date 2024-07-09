// Room: /u/zqb/tiezhang/sslu-2.c

inherit ROOM;

void create()
{
	set("short", "碎石路");
	set("long", @LONG
你走在一条用碎石铺成的小路上。两旁山势低缓，长满了翠绿的毛竹，阵
阵微风吹过，耳旁便响起一片沙沙声。
LONG
	);

	set("exits", ([ 
  "eastup" : __DIR__"sslu-3.c",
  "westdown" : __DIR__"sslu-1",
]));
	set("no_clean_up", 0);
	set("outdoors", "tiezhang");

	set("coor/x",-310);
  set("coor/y",-200);
   set("coor/z",50);
   setup();
}
