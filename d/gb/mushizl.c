// mushizl.c 墓室走廊

inherit ROOM;

void create()
{
         set("short", "墓室通道");
       set("long", @LONG
这里是墓室里的一条通道，墙壁上很潮湿。四下里很黑，前面隐隐传来人
声。幸好通道不是很长，所以有一丝光亮从远处照过来，把人的影子拖的长长
的,印在地上。
LONG
       );
       set("exits", ([
        "south" : __DIR__"fenduo2",
        "west" : __DIR__"xiao-mushi",
    	]));
	set("coor/x",110);
  set("coor/y",70);
   set("coor/z",-10);
   setup();
     }
