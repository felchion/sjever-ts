// road1.c 小路

inherit ROOM;

void create()
{
        set("short", "小路");
        set("long", @LONG
这条小路是胆大的男人们去密林里打猎时走出来的，再往西走就到草原边
缘而进入森林了。偶尔可以听见远处传来野兽的吼声，你还是快回去吧。
LONG
        );

        
        set("exits", ([
            "westup" : __DIR__"shanqiu",
            "east" : __DIR__"pmchang",            
        ]));

        set("outdoors", "草原");
        
        set("coor/x",-330);
  set("coor/y",100);
   set("coor/z",0);
   setup();
}
