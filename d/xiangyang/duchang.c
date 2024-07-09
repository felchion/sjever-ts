// Room: d/xiangyang/duchang.c

#include <ansi.h>

inherit ROOM;


void create()
{
	set("short", HIY"万金赌坊"NOR);
	set("long",
"这是方圆百里远近闻名的一间大赌坊，平日里生意非常红火，经常是座无\n"
"虚席。最近已经衰落，关门歇业了。\n"
	);
	set("indoors", "襄阳");
	set("no_fight", 1);	

	set("exits", ([
		"west" : __DIR__"bjie1",
	]));

	set("objects", ([
		__DIR__"npc/duke" : 2,
		__DIR__"npc/dclaoban" : 1,
	]));

	set("incity",1);
	setup();
}
