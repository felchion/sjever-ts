// sroom.c 睡房
inherit ROOM;

void create()
{
        set("short", "哈萨克帐篷");
        set("long", @LONG
这是哈萨克族人居住的帐篷，帐篷内用羊皮铺地，四周也挂了好些羊皮牛
皮，左边是一张用干草垫底的羊毛床，入口有一小火盆。看起来主人似乎不在
家，你如果累了就歇息(sleep)一下吧。
LONG);

        set("sleep_room", "1");
        set("no_fight", "1");
        set("exits", ([            
              "southeast" : __DIR__"shop",            
        ]));                
        set("coor/x",-330);
  set("coor/y",120);
   set("coor/z",0);
   setup();
}
