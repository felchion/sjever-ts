// /d/xiangyang/jianghu.c 江湖客栈
// Lklv 2001.9.24

inherit ROOM;

void create()
{
        set("short", "江湖客栈");
        set("long", @LONG
这里是襄阳城内最大的一家客栈，店里的生意看起来很红火。很多南北往
来的客人在这里投宿。客栈门口旁边的拴马桩上拴着几匹马，一个年纪不大的
店小二正在招呼客人。见你走了过来，店小二急忙迎上来打着招呼。客栈南面
的墙上挂有一块牌子(paizi)。
LONG
        );

        set("no_sleep_room",1);
        set("no_fight",1);
        set("item_desc", ([
                "paizi" : "楼上休息，白银五两。\n",
        ]));

        set("objects", ([
                __DIR__"npc/kzxiaoer" : 1,
        ]));

        set("exits", ([
                "south" : __DIR__"bcx3",
                "up" : __DIR__"kezhan2",
        ]));
        set("incity",1);
        setup();
}

int valid_leave(object me, string dir)
{
        if ((string)me->query_temp("rent_paid")!="江湖客栈" && dir == "up" )
                return notify_fail("楼上一个声音喊道：“客官，您还没给钱！”\n");
        if ((string)me->query_temp("rent_paid")=="江湖客栈" && dir == "south" )
                return notify_fail("小二道：“客官已经付了银子，怎么不住店就走了呢！旁人还以为小店伺候不周呢！”\n");
        return ::valid_leave(me, dir);
}