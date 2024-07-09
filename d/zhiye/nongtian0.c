// 农田入口
// nongtian0.c
// augx@sj 10/17/2001

#include <ansi.h>
#include <room.h>
inherit ROOM;

void create()
{
	set("short","农田口");
	set("long",
"这是片一眼望不到头的农田，大部分田地上都已经种上了庄稼，一副繁荣
的景象。少数农田还空着，你可以在上面种植自己的植物。但是这里的田地看
起来都一摸一样，不熟悉此地的人很容易迷失方向，好在官府已经在田边路口
竖立了指示牌以指明方向，同时还派巡捕巡查山路，防止凶杀事件。
"); 

	set("objects",([
		__DIR__"npc/xunbu" : 2,
	]));
        
	set("exits",([
		"north"		: "/d/changan/northroad4.c",
		"southwest"	: __DIR__"nongtian1",
		"southeast"	: __DIR__"nongtian2",
		"northeast"	: __DIR__"nongtian3",
	]));
	
	set("outdoors", "长安郊外");
	set("incity",1);
	setup();
}

int clean_up()
{
	return 1;
}

int valid_leave(object me, string dir)
{
	if( dir == "north" )
		me->delete_temp("nongtian1");
	else
		me->set_temp("nongtian1",random(4));

	if ( me->query("no_quest") && dir != "north")
		return notify_fail("");

	return ::valid_leave(me , dir);
}
