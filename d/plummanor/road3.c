// Room: /d/plummanor/road3.c
// By Jpei

inherit ROOM;

#include "make_maze.c"

void create()
{
	set("short", "青石板大路");
	set("long", @LONG
这里已经到了梅林前面，只见遍地都是梅树，枝叶茂密。想象初春梅花盛
开之时，香雪如海，定然观赏不尽。南面是一座朱门白墙的大庄院。
LONG
	);
	set("outdoors","梅庄");

	set("item_desc", ([
		"north" : "梅花掩映，你看不清那边的情况！\n",
	]) );
	set("exits", ([
		"south" : __DIR__"entrance",
		"north" : __DIR__"plum_maze",
	]));
	setup();
}

int valid_leave(object me, string dir)
{
	int can_leave;
	object ob;

	can_leave = ::valid_leave(me, dir);
	if (dir == "north" && can_leave) {
/*		if (me->query_skill("qimen-bagua", 1) > 89)
			write("由于你平常看惯了这些简易的阵法，所以不用细想，信步就走出了阵！\n");*/
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
		me->set_temp("plum_maze/x", me->query_temp("plum_maze/south_exit") + 1);
		me->set_temp("plum_maze/y", MAZE_HEIGHT);
	}
	return can_leave;
}
