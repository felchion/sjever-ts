//stoneroad3.c

inherit ROOM;

void create()
{
	set("short", "石板路");
        set("long",@LONG
一条平整宽阔的石路，路边是两长溜僧棚，可供大家休息之用。香客们
手执线香，缓步通过。一些忙碌的僧侣，急匆匆的奔驰而去，似乎有什么要
事在身。一队护寺僧兵手执戒刀，正自来往巡视。
LONG
	);
        set("outdoors", "shaolin");
	set("exits", ([
             "east" : __DIR__ "brtang",
             "north" : __DIR__"celang2",
             "west" : __DIR__"stoneroad2", 	  
          ]));
	set("coor/x",60);
  set("coor/y",270);
   set("coor/z",110);
   setup();
}



