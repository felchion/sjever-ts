// Room: xiaohe.c
// Looklove y2k/4/26

inherit ROOM;

void create()
{
        set("short","小河");
        set("long", @LONG
一条弯弯曲曲的小河，河上架着个独木桥，你走在桥上，脚下发出吱吱的
声音。远处有几个顽皮的小孩子在河边嬉戏。桥对面是一家很大的宅院。可以
看见一扇朱红的大门。南边是柳宗镇。
LONG
        );

        set("exits", ([
                "south" : __DIR__"liuzz",
                  "north" : __DIR__"xue_men",
        ]));
 
        set("coor/x",-40);
  set("coor/y",30);
   set("coor/z",0);
   setup();
}

