//huayuan.c
//Csl 2000.04.26

inherit ROOM;
void create()
{
        set("short", "花园");
        set("long",@LONG
这个花园虽然不大，但整理的井井有条，种植着各式各样的花花草草，看
的出这里的主人下了一番功夫，常有人来这里散步。沿着弯弯曲曲的花径，可
以通向后院。
LONG
        );
	set("outdoors", "襄阳");
        set("exits",([
                "south"  :  __DIR__"houyuan",
	]));

	setup();
}