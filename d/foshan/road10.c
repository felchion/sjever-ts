//by xiaoyao

inherit ROOM;
void create()
{
        set("short","林间道");
        set("long",@LONG
这里是一条林间小道，在树丛中蜿蜒。岭南山林茂密，多产毒
虫，各位行路要十分小心。
LONG);
        set("objects", ([
           __DIR__"obj/hua3": 1,
        ]));
        set("outdoors", "佛山");
        set("exits",([ /* sizeof() == 1 */
            "northeast" : __DIR__"road11",
            "southeast" : __DIR__"road12",
            "west"      : __DIR__"road9",
        ]));
        set("coor/x",70);
  set("coor/y",-530);
   set("coor/z",0);
   setup();
}

