// /d/xiangyang/xiyuan.c 戏院
// Lklv 2001.9.26

inherit ROOM;
void create()
{
        set("short", "戏院");
        set("long", @LONG
这里是一家戏院，场子里摆了很多凳子，正前方的看台上有一个戏子正在
咿咿呀呀地吊嗓子。看台的一角摆有一个方桌，上面放有一个大茶壶。现在不
是营业时间，没有人来。
LONG
        );

        set("exits", ([
                "west" : __DIR__"dcx4",
        ]));

        set("objects", ([
                __DIR__"npc/xizi" : 1,
        ]));
        set("incity",1);
	setup();
}