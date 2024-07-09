inherit ROOM;

void create()
{
        set("short", "蒙古军营");
        set("long", @LONG
这里是蒙古攻击襄阳的临时军营，时时有士兵来回巡逻。
往西南便能到达襄阳。
LONG
        );
        set("outdoors", "襄阳");
        set("objects", ([
                __DIR__"npc/weishi" : 3,
        ]));

        set("exits", ([
		"southwest" : __DIR__"outnroad2",
                "enter" : __DIR__"yingzhang",
        ]));

	setup();
}
