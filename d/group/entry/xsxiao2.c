// xsxiao2.c

inherit ROOM;

#include <ansi.h>
#include "/d/group/entry.h"

void create()
{
	set("short", "湖边小路");
	
	set("long", "天湖湖边的一条小路，向前望不到尽头，不知道通向那里。路边野花芬芳，\n"
		"蝴蝶飞舞，除了你自己的脚步声，四下里只听见一声高一声低的鸟鸣。\n");

	set("exits", ([
		"east":"/d/xueshan/xiaolu3",
		"northwest":"/d/xueshan/xiaolu1",
		]));

	set("outdoors","大雪山");
	
	set("group",1);
	
	setup();
}
