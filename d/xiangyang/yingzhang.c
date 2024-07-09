inherit ROOM;

void create()
{
        set("short", "营帐中");
        set("long", @LONG
这是蒙古攻击襄阳的临时军营，营帐中几个披挂着盔甲的将军走来走去正
在和一个看起来样子象谋士的人大声的谈论、争执着什么。营帐外有几个士兵
在巡逻。
LONG
        );
        set("outdoors", "襄阳");
        set("objects", ([
		__DIR__"npc/weishi" : 3,
        ]));

        set("exits", ([
                "out" : __DIR__"junying",
        ]));

        set("no_fight",1);

	setup();
}