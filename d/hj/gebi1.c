// gebi1.c
#include <room.h>
inherit ROOM;
void create()
{
       set("short","大戈壁");
       set("long", @LONG
这是西域的大戈壁，要走出这里并非易事。不远处尘土沙粒铺天盖地般吹
来，看来要起风暴了。
LONG);
        set("exits", ([
                "north" : __DIR__"gebi2",
                "south" : __DIR__"gebi3",
                "east" : __DIR__"gebi4",
                "west" : __FILE__,
        ]));
        set("outdoors", "西域");
        set("coor/x",-310);
  set("coor/y",150);
   set("coor/z",0);
   setup();
}

#include "gebi.h";
