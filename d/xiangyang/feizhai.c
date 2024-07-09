// /d/xiangyang/feizhai.c 废宅
// Lklv 2001.9.26

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", "废宅");
        set("long", @LONG
这是一个荒废的宅院，院子里长满了青草和不知名的各种低矮灌木丛。崩
塌的黄土墙破败不堪，和外面繁华热闹的街道形成鲜明的对比。这里平常没有
什么人来，成了叫化子们休息的地方。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "west" : __DIR__"dcx1",
        ]));
        set("objects", ([
		CLASS_D("gaibang/xuxiaowu") : 1,
		CLASS_D("gaibang/xiang") : 1,
        ]));
        set("incity",1);
	setup();
}