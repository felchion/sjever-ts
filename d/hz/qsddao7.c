// /u/beyond/hz/qsddao4.c 青石大道
 
inherit ROOM;
void create()
{
        set("short","青石大道");
        set("long", @LONG
你走在一条林荫大道上，两边鸟语花香，游客们往来穿梭，熙熙攘攘好
不热闹。远处就是杭州城了，你也不禁加快脚步，杭州北门展现在你的面前。
LONG
        );
set("outdoors","hz");
        set("exits", ([
            "south" : __DIR__"beimen",
            "north" : __DIR__"shanlu3",
            
]));
        set("coor/x",100);
  set("coor/y",-300);
   set("coor/z",0);
   setup();
}
