// shamo1.c 大沙漠
// Modify By River@SJ 99.06
#include <ansi.h>
#include <room.h>
inherit ROOM;
void create()
{ 
       set("short",YEL"大沙漠"NOR);
       set("long", @LONG
这是一片一望无际的大沙漠的入口。你一进来就迷失了方向。在冬天时能
看见一片片的积雪，西边是一个个小沙丘。
LONG);    
	set("exits", ([
		"west" : __DIR__"shaqiu1",				
                "northeast" : "/d/xingxiu/silk6"
	]));             
        set("outdoors", "丝绸之路");
        set("coor/x",-290);
  set("coor/y",90);
   set("coor/z",0);
   set("coor/x",-290);
 set("coor/y",90);
   set("coor/z",0);
   setup();
}
