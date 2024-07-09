// Room: /d/putian/road08.c
// Lklv 2001.9.11

inherit ROOM;
void create()
{
        set("short", "石阶");
        set("long", @LONG
窄窄的石阶继续在向山上延伸，山风吹来，带来一阵阵凉意。山势虽然不是很
陡峭，但是也有些吃力。眼看着三两个僧人健步如飞，从你身旁经过，看来好象身
怀高深武功。
LONG
        );

        set("exits", ([
                "northdown" : __DIR__"road07",
                "southup" : __DIR__"shanmen",
        ]));

        set("objects",([
        	__DIR__"npc/jxiang-ke.c" : 2,
        ]));

        set("outdoors", "福州");
        setup();
}

