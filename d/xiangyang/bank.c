// Room: /xiangyang/bank.c
// Looklove y2k/4/09

inherit ROOM;

void create()
{
        set("short", "宝龙斋");
        set("long", @LONG
这是一家老字号的钱庄，已有几百年的历史，在全国各地都有分店。它发
行的银票信誉非常好，通行全国。店内摆设十分简单，一个高高的柜台上面装
了铁栅栏。
LONG
        );

        set("exits", ([
                "east" : __DIR__"njie3",
        ]));

        set("objects", ([
                __DIR__"npc/banker" : 1,
        ]));

        set("incity",1);
	setup();
}
