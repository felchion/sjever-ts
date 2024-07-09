// hsroad2.c

inherit ROOM;

void create()
{
	set("short", "青石大道");
	set("long", @LONG
你走在一条青石大道上，南边隐约可见到扬州城的城墙。北面是通往京城
的驿道，西北面一条土路通向山里。
LONG
	);
        set("outdoors", "wudang");

	set("objects", ([
		__DIR__"npc/dadao" : 1,
	]) );
	set("exits", ([
                "west" : "/d/huanghe/tiandi4",
            "east" : "/d/taishan/yidao2",
	]));

	set("coor/x",150);
  set("coor/y",100);
   set("coor/z",0);
   setup();
}

