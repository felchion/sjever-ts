// zl1.c 长廊
// Modify By River@SJ 99.06
#include <ansi.h>
#include <room.h>
inherit ROOM;
void create()
{ 
       set("short",HIC"长廊"NOR);
       set("long", @LONG
这是一条走廊。一边是青砖墙壁，另一边是古木雕成的围栏虽是普通过道，
却也隐射出江湖第一大教制端严。东面是广场，西面是练武场。
LONG);    
	set("exits", ([
                "east" : __DIR__"guangc",
		"west" : __DIR__"wu2",
	]));             
        set("outdoors", "明教光明顶");     
        set("coor/x",-240);
  set("coor/y",240);
   set("coor/z",120);
   set("coor/x",-240);
 set("coor/y",240);
   set("coor/z",120);
   setup();
}
