// /d/xiangyang/zrdian.c 猪肉店
// Lklv 2001.9.24

inherit ROOM;
void create()
{
        set("short", "猪肉店");
        set("long", @LONG
这是一家卖猪肉的小铺子，店面不算很大，一张油腻腻的肉案子横在地当
中，案子后面站了一位胖乎乎的年轻人，他身上扎了个围裙，围裙上一圈圈的
油印。他正站在案子后面向街上望着。
LONG
        );

        set("exits", ([
                "south" : __DIR__"bcx2",
        ]));

        set("objects", ([
		__DIR__"npc/zhurour" : 1,
        ]));
        set("incity",1);
	setup();
}