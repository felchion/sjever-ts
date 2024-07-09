// Room: /dali/yaopu.c
// Modify By River 98/12
inherit ROOM;
#include <ansi.h>
void create()
{
	set("short", HIC"药铺"NOR);
	set("long", @LONG
这是一家药铺，一股浓浓的药味让你几欲窒息，那是从药柜上的几百个小
抽屉里散发出来的。药店掌柜独自坐在茶几旁，独自喝着茶，看也不看你一眼。
一名小伙计站在柜台后招呼着顾客。柜台上贴着一张发黄的纸片。
LONG
	);
	set("objects", ([
	//	"/d/zhiye/obj/yaolu" : 1,
		"/d/zhiye/npc/yaodian-zhanggui" : 1,
	]));

	set("exits", ([
		"west" : __DIR__"dongjie2",
	]));
	set("no_fight", 1);
	set("no_zhiye", 1);
	set("incity", 1);
	setup(); 
}

void init()
{
	object me = this_player();
	if(me->query_temp("xuncheng"))
		me->set_temp("dali_xc/yaopu",1);
}
