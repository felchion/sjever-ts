// /d/city/jiashan.c
// update 1997.7.4
// this is made by beyond

#include <ansi.h>
inherit ROOM;
void create()
{
	set("short",HIY"假山"NOR);
	set("long",@long
由复廊和小道就到了这里，黄色的山拔地而起，山峰俊俏，山间古柏，山
顶小亭，山中还有小院、石桥、石室等，气势磅礴。为江南山石中所少见。
long);
	set("exits",([
		"east" : __DIR__"geyuan",
	]));

	set("objects",([
		__DIR__"npc/binv" : 1,
		BINGQI_D("shizi") :1,
	]));

	set("incity",1);
	setup();
}
