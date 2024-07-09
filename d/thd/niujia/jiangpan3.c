// Room: /u/jpei/thd/jiangpan3.c

inherit ROOM;
#include "nadir.c"

void create()
{
	set("short", "江畔");
	set("long", @LONG
一条大江横亘在面前，江水浩浩，日日夜夜无穷无休地奔向大海。
LONG
	);
	set("no_clean_up", 0);
	set("outdoors","牛家村");

	set("exits", ([
		"west": __DIR__"jiangpan2.c",
        "east": __THD__"haibin.c",
	]) );

	setup();
}

