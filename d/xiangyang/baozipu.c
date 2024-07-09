// Room: /d/xianyang/baozipu.c 包子铺
// Lklv 2001.9.22

inherit ROOM;
void create()
{
        set("short", "包子铺");
        set("long", @LONG
这是一家包子铺，附近的人们都喜欢吃这里的包子，老板包子王的手艺也
是远近闻名。门口有个火炉，上面放了一摞笼屉，不断冒出一丝丝的热气。
LONG
        );

        set("exits", ([
                "east" : __DIR__"xcx4",
        ]));

        set("objects", ([
		__DIR__"npc/baoziwang" : 1,
        ]));

        set("incity",1);
	setup();
}