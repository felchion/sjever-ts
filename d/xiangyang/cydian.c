// /d/xiangyang/cypu.c 成衣铺
// Lklv 2001.9.26

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", "成衣铺");
        set("long", @LONG
这里是一家成衣铺，店面很小，门前的树墙上晾着一件对襟小褂。屋子里
也不是很宽敞，进门对面的架上挂着几件长袍。几个唧唧喳喳的姑娘围着一件
旗袍在议论着什么。
LONG
        );

        set("item_desc", ([
                "paizi" : "只供成衣，恕不定做\n",
        ]));

        set("exits", ([
                "west" : __DIR__"dcx3",
        ]));

        set("objects", ([
		__DIR__"npc/yusan" : 1,
        ]));

        set("incity",1);
	setup();
}