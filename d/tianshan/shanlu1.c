// Room: /d/tianshan/shanlu1.c
// By Linux

inherit ROOM;

void create()
{
	set("short", "天山山路");
	set("long", @LONG
这是一条通向山上的小路，因为此地长年苦寒，路的两边只有些耐寒的草
本植物生长。并无半点虫鸣鸟叫。只有山风呼啸，阵阵寒意袭来，你不由的裹
紧了身上的衣服。
LONG);
	 set("exits", ([
                "east" :__DIR__"kongchang",
	        "northup" :__DIR__"shanlu2",
        ]));       
        set("objects", ([
                __DIR__"npc/zhuji" : 1,
        ]));    

	set("outdoors", "天山");
	setup();
}
