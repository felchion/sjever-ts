// Room: /u/xdd/gumu/gchang.c
// Modify By River 98/08
inherit ROOM;
#include <ansi.h>
void create()
{
        set("short",HIY"乡间广场"NOR);
        set("long", @LONG
此处是乡村小镇村民们打谷晒粮的地方，来了戏团或每年正月庙会，更是
乡镇的集会所，平日里常有孩子在此玩耍，东南面有个小集市。
LONG        );

        set("outdoors","古墓");

        set("exits", ([
         "northwest" : __DIR__"xiaolu2",
         "southeast" : __DIR__"jishi",
        ]));

        set("coor/x",0);
  set("coor/y",20);
   set("coor/z",0);
   setup();
}
