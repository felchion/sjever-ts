// Room: /u/jpei/thd/chufang.c

inherit ROOM;

void create()
{
	set("short", "厨房");
	set("long", @LONG
这是桃花岛的厨房。黄药师不知使用了什么方法抓了几个御厨来，正在忙
碌着。屋中充斥着青菜和调料的味道。几个厨师正在忙碌着。偶尔会有些吃不
完的主食和做菜时剩下的猪肉留在这里。
LONG
	);
	set("no_clean_up", 0);

	set("objects",([
		__DIR__"obj/meat" : 1,
		__DIR__"obj/rice" : 1,
	]));
	set("exits", ([
		"east" : __DIR__"neishi2.c",
		"south" : __DIR__"fanting.c",
	]));

	setup();
}


