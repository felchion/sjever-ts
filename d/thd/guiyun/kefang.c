// /u/jpei/guiyun/kefang.c

inherit ROOM;

void create()
{
	set("short","客房");
	set("long",@LONG
这是招待客人住宿的房间。室内陈设精雅，两床相对，枕衾雅洁。
LONG
);
	set("exits",([
		"south" : __DIR__"zoulang2",
	]) );
	set("no_clean_up", 0);
        set("no_fight", 1);
	set("sleep_room", 1);
	setup();
}
