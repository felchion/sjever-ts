// /d/xiangyang/tanxi.c 檀溪
// Lklv Modify 2001.9.22

inherit ROOM;
void create()
{
	set("short", "檀溪");
	set("long", @LONG
这是一条湍急的小河，河水并不甚宽，清澈见底。几个赤脚经过这里的乡
民告诉你，这里就是当年刘皇叔躲避追兵时马越檀溪的地方，这条小河就是鼎
鼎有名的檀溪。西南边有条小路通往四川。
LONG
	);
        set("outdoors", "襄阳");

	set("exits", ([
		"east" : __DIR__"outwroad1",
		"southwest" : __DIR__"xiaolu1",
	]));
	setup();
}