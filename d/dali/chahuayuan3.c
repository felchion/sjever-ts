// Room: /dali/chahuayuan3.c
// Modify By River 98/12
inherit ROOM;
#include <ansi.h>
void create()
{
        set("short",HIC"茶花园"NOR);
      	set("long", @LONG
你走在一个小山坡上，这里到处都是盛开的鲜花，你有点眼花缭乱了，你
漫步在这花园中，心情舒畅许多。 
LONG
        );
        set("exits", ([ 
              "northwest" : __DIR__"chahuayuan4",
              "northeast" : __DIR__"chahuayuan5",
              "southdown" : __DIR__"chahuayuan2",
	]));
        set("outdoors","大理");
        set("objects", ([
              __DIR__"obj/chahua3" : 1,
        ]));
        set("coor/x",-280);
  set("coor/y",-420);
   set("coor/z",0);
   setup();

}

