// shenchu2.c  树林深处
// Modify By River@SJ 99.06
#include <ansi.h>
#include <room.h>
inherit ROOM;
void create()
{ 
       set("short",GRN"树林深处"NOR);
       set("long", @LONG
这是乱树林深处。几十丈高的树木簇在一块，密实的枝叶象一蓬蓬巨伞般
伸向天空，把阳光遮得丝毫也无。你一走进这这觉得阴阴森林的，你的感觉告
诉你，得赶快离开，不能再深入了。
LONG);    
	set("exits", ([
		"west" : __DIR__"shenchu1",			
		"northeast" : __DIR__"shenchu3",
                "southeast" : __DIR__"shenchu4",
                "north" : __DIR__"shenchu5",
	]));   
	set("objects",([
		MEDICINE_D("yaocao") : 1,
                BINGQI_D("da-shugan") : 1,
	]));
        set("outer_times", 4);
        set("outdoors", "明教光明顶");
        set("coor/x",-310);
  set("coor/y",150);
   set("coor/z",100);
   setup();
}

#include "/d/mingjiao/npc/attacker.h";
