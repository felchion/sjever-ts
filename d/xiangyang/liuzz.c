// Room: liuzz.c
// Looklove y2k/4/26

inherit ROOM;

void create()
{
        set("short","柳宗镇");
        set("long", @LONG
欢迎来到柳宗镇。这个镇子不大，但是在武林中却很有名气，因为这个镇
子上住着一位大大有名的神医，人们谈论起他来，脸上都情不自禁露出自豪的
神色。西边有一家小酒馆，再往北边走是一条小河。
LONG
        );

        set("exits", ([
          "south" : __DIR__"xlu_lzz",
          "north" : __DIR__"xiaohe",
          "west" : __DIR__"jiuguan",
        ]));
 
        set("coor/x",-40);
  set("coor/y",20);
   set("coor/z",0);
   setup();
}

