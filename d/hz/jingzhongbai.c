// /u/beyond/hz/jingzhongbai.c 精忠柏
#include <ansi.h>
inherit ROOM;
void create()
{
        set("short",GRN"精忠柏"NOR);
        set("long", @LONG
墓园的西部有一“精忠柏”亭，乃以“化石”来比喻岳飞坚贞不屈的精
神，反映了人民对这位民族英雄的崇敬、景仰之情。
LONG
        );
        set("exits", ([
          "west" : __DIR__"yuanmen",
          "south" : __DIR__"mu",
]));
        set("coor/x",100);
  set("coor/y",-350);
   set("coor/z",10);
   setup();
}
