// Room: jiuguan.c
// Looklove y2k 4/26

inherit ROOM;

void create()
{
        set("short", "四季小吃店");
        set("long", @LONG
这是一家小酒馆，店里空荡荡的。没有多少人来这里吃饭，所以生意不是
很兴隆。屋子里零零散散摆着几张桌椅。几个无聊的闲人围在一个过路的外乡
客人周围，听他在讲山外面的新鲜事。
LONG
        );

        set("exits", ([
                   "east" : __DIR__"liuzz",
        ]));

        set("objects", ([
                __DIR__"npc/xiaoer" : 1,
        ]));

        set("coor/x",-50);
  set("coor/y",20);
   set("coor/z",0);
   setup();
} 

