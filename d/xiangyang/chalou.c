// Room: d/xiangyang/chalou.c 天香楼
// Lklv 2001.9.25

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", HIC"天香楼"NOR);
        set("long", @LONG
这是襄阳城里最大的一家茶楼。只见屋内几张长条桌排开，凳子上坐满了
各种口音的茶客，或高声谈笑，或交头接耳，将来自天南海北的声音混杂在了
一起。茶楼里飘散着一股淡淡的茶香，闻着格外舒服。
LONG
        );
        set("resource/water", 1);

        set("exits", ([
                "west" : __DIR__"bjie2",
                "up" : __DIR__"chalou2",
        ]));

        set("item_desc", ([
                "up" : "楼上的人很多，在这里看不清楚。\n"
        ]));
        set("objects", ([
                __DIR__"npc/chaboshi" : 1,
        ]));
        set("incity",1);
	setup();
}
int valid_leave(object me, string dir)
{
	if ( me->query("id") == "cha boshi" &&  dir == "west" ) return 0;
        return ::valid_leave(me, dir);
}