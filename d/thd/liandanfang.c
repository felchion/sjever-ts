// /u/jpei/thd/liandanfang.c

inherit ROOM;

void create()
{
	set("short", "炼丹房");
	set("long", @LONG
这是桃花岛主黄药师的秘密练丹房。
LONG
	);
	set("exits", ([
	      "out" : __DIR__"jingshe.c",
	]) );
	set("objects", ([
        MEDICINE_D("jiuhuawan"): 1,
	]));
	set("no_clean_up",0);
	setup();
}

