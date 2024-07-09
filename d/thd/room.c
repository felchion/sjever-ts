// Modify by Lklv 2001.10.16

#include <ansi.h>
inherit ROOM;

void create()
{
	set("short", HIB "箫房" NOR);
	set("long", "这里是桃花岛的一个小房间，放着一些乐器和兵器。\n");
	set("sleep_room",1);
	set("exits", ([
		"out" : __DIR__"jicui.c",
	]) );
	set("objects",([
		BINGQI_D("sword/biyuxiao") : 1,
		BINGQI_D("tiexiao") : 1,
		BINGQI_D("yuxiao") : 1,
		BINGQI_D("xiao") : 1,
		BINGQI_D("sword/ziyuxiao") : 1,
	]));
	setup();
}
