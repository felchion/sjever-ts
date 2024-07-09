// Room: /d/fuzhou/road3.c

inherit ROOM;

void create()
{
	set("short", "山路");
	set("long", @LONG
你走在一条山路上。时值春日，路旁的山坡上开满了金黄色的油菜花。向
南则进入福建武夷山区。
LONG
	);

	set("exits", ([
                "northeast" : __DIR__"road2",
                "south" : __DIR__"road4",
	]));

	set("outdoors", "福州");
	set("coor/x",100);
  set("coor/y",-430);
   set("coor/z",0);
   setup();
}

