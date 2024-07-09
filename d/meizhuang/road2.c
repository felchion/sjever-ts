// Room: /d/plummanor/road2.c
// By Jpei
// Update by Lklv 2001.10.9

inherit ROOM;
#include "make_maze.c"

void create()
{
	set("short", "小路");
	set("long", @LONG
这里已经到了梅林前面，只见遍地都是梅树，枝叶茂密。想象初春梅花盛
开之时，香雪如海，定然观赏不尽。北面的一条小路折向西方。
LONG
	);
	set("outdoors","梅庄");

	set("item_desc", ([
		"south" : "梅花掩映，你看不清那边的情况！\n",
	]) );
	set("exits", ([
		"north" : __DIR__"road1",
		"south" : __DIR__"plum_maze",
	]));
	setup();
}

int valid_leave(object me, string dir)
{
	int can_leave;
	object ob;

	can_leave = ::valid_leave(me, dir);
	if (dir == "south" && can_leave) {
		if (objectp(ob = me->query_temp("follow"))) {
			me->set_leader(0);
			tell_object(me, ob->name() + "走进梅林，一晃就不见了。\n");
			return 0;
		}
		if (me->query("env/brief")) {
			me->delete("env/brief");
			tell_object(me, "这片梅林幽深曲折，你决定取消行走时的简短描述模式。\n");
		}
		make_maze(me);
		me->set_temp("plum_maze/x", me->query_temp("plum_maze/north_exit") + 1);
		me->set_temp("plum_maze/y", 1);
	}
	return can_leave;
}
