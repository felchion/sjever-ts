 // muchang1.c 牧场
 
inherit ROOM;
void create()
{
        set("short","牧场");
        set("long",@LONG 
这里是一片茫茫无际的牧场，一眼望不到边的草原，远处能看到高耸的雪
山。附近能看到藏民们正在放牧牛马，小孩子们在奔跑玩耍。
LONG);
        set("exits",([
                "north" : __DIR__"muchang1",
                "south" : __DIR__"muchang2",
                "west" :__DIR__"muchang1",
                "east" :__DIR__"muchang4",
        ]));
            set("objects",([__DIR__"npc/zangao" : 1,]));
        set("coor/x",-200);
  set("coor/y",170);
   set("coor/z",-10);
   setup();
}
