// yuchuan4.c 丐帮渔船

#include <ansi.h>
inherit ROOM;

void create()
{
	set("short","渔船");
	set("long", @LONG
渔船都连在一齐，而且都几乎是一模一样的，看得你头晕眼花。忽然
一个暗浪涌来，你一个不稳，‘啊。。。’，你几乎掉下了海去。
LONG
	);
	set("exits", ([
	"west" : __FILE__,
	"north" : __FILE__,
	"east" : __DIR__"yugang",
	"south" : __DIR__"yuchuan5",
	]));
	setup();
}

