// /u/jpei/guiyun/wofang.c

inherit ROOM;

void create()
{
	set("short","卧房");
	set("long",@LONG
这是少庄主的卧房。床榻上铺着锦被，一位女子羞答答的坐在床边。
LONG
);
	/*  set("object",([
	__DIR__"obj/luchenfen.c" :     1,
	]));  */
	set("exits",([
        "south" : __DIR__"zoulang3",
	]) );
    set("objects",([
          __DIR__"npc/cheng" : 1,
      ]));
	set("no_clean_up",0);
	setup();
}
