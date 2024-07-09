// Room: /dali/chahuayuan2.c
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
              "northup" : __DIR__"chahuayuan3",
              "southdown" : __DIR__"chahuayuan",
	]));
        set("outdoors","大理");
        set("objects", ([
             __DIR__"obj/chahua2" : 1,
        ]));
        set("coor/x",-280);
  set("coor/y",-430);
   set("coor/z",-10);
   setup(); 
}

int valid_leave(object me, string dir)
{
/*      object *ob = deep_inventory(me);
        int i = sizeof(ob);
        if(dir == "southdown"){
        while (i--)
        if( ob[i]->id("flower") && ob[i] > 2)
          return notify_fail("你带这么多花出去干吗？\n");               
          }*/
	if ( dir == "southdown" )
        me->delete_temp("flower_paid");
	return ::valid_leave(me, dir);
}
