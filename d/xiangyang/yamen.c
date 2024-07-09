// Room: /d/city/yamen.c
// Lklv 2001,9,25

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", HIR"衙门"NOR);
        set("long", @LONG
这里是衙门大门，两扇朱木大门紧紧关闭着。“肃静”“回避”的牌子分
放两头石狮子的旁边。前面有一个大鼓，显然是供小民鸣冤用的。几名衙役在
门前巡逻。
LONG
        );

        set("exits", ([
                "east" : __DIR__"njie1",
                "north" : __DIR__"ymzhengting",
                "south" : __DIR__"chucshi",
        ]));

        set("objects", ([
                __DIR__"npc/yayi" : 2,
        ]));

	set("incity",1);
	setup();
}

int valid_leave(object me, string dir)
{
        if (!wizardp(me) && objectp(present("ya yi", environment(me))) && dir != "east"){
        	if( me->query_condition("killer"))
        		return notify_fail("衙役喝道：“你是谁？快快离开这里。”\n");
        	if( !objectp(present("tiebu wenshu", me)) )
                	return notify_fail("衙役喝道：“威……武……。”\n");
        }
        return ::valid_leave(me, dir);
}