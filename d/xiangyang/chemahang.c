// Room: /d/city/chemahang.c 车马行

inherit ROOM;

void create()
{
	set("short", "车马行");
	set("long", @LONG
这是一个车马行，专门做拉脚的生意，院子里停了几辆大车，靠西墙
的位置有个马厩，里面栓了几匹马。旁边堆有一堆干草。
LONG
	);

	set("exits", ([
                "south" : __DIR__"ncx2",
        ]));

	set("incity",1);
	setup();
}