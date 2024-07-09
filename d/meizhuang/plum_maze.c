// Room: /d/meizhuang/plum_maze.c //梅林单路径迷宫
// By Jpei

inherit ROOM;

#include <ansi.h>
#include "make_maze.c"

int do_look(string arg);
int do_set(string arg);
int do_follow(string arg);

void create()
{
	set("short", "梅林");
	set("long", @LONG
这是梅林中的一条小路。四处遍地都是梅树，枝叶茂密地连天空都被遮住
了，很难看清周围的情况。
LONG
	);
	set("outdoors","杭州");

	set("exits", ([
		"north" : __FILE__,
		"south" : __FILE__,
		"west" : __FILE__,
		"east" : __FILE__,
	]));
	setup();
}

void init()
{
	object me = this_player();
	int x, y;

	x = me->query_temp("plum_maze/x");
	y = me->query_temp("plum_maze/y");
	if (!x || !y) {
		make_maze(me);
		me->set_temp("plum_maze/x", random(MAZE_WIDTH) + 1);
		me->set_temp("plum_maze/y", random(MAZE_HEIGHT - 6) + 4);
	}
	add_action("do_look", "look");
	add_action("do_set", "set");
	add_action("do_follow", "follow");
	set("exits", ([
		"north" : __FILE__,
		"south" : __FILE__,
		"west" : __FILE__,
		"east" : __FILE__,
	]));
}

int do_set(string arg)
{
	if (arg && arg[0..4] == "brief") {
		write("这片梅林幽深曲折，你还是不要使用行走时的简短描述模式吧。\n");
		return 1;
	}
	return 0;
}

int do_follow(string arg)
{
	object ob, me = this_player();

	if (!arg || arg == "none") return 0;
	if (!objectp(ob = present(arg, environment(me)))) return 0;
	if (ob == me || !ob->is_character()) return 0;
	write("这片梅林幽深曲折，你是很难跟着另一个人行走的，还是别白费力气了。\n");
	return 1;
}

int do_look(string arg)
{
	object *inv, env, me = this_player();
	int i, x, y, can_n, can_e, can_w, can_s, path, exit_num;
	string str;

	x = me->query_temp("plum_maze/x") - 1;
	y = me->query_temp("plum_maze/y") - 1;
	path = me->query_temp("plum_maze/" + x + "_" + y);
	can_n = path & LINK_UP;
	can_w = path & LINK_LEFT;
	can_e = path & LINK_RIGHT;
	can_s = path & LINK_DOWN;
	exit_num = 0;
	if (can_n) exit_num++;
	if (can_w) exit_num++;
	if (can_e) exit_num++;
	if (can_s) exit_num++;

	if (!arg) {
		env = this_object();
		if (x == me->query_temp("plum_maze/north_exit") && y == 0) {
			can_n = 1;
			exit_num++;
			set("long", @LONG
这是梅林中的一条小路，小路往北就出了梅林，在远处似乎折向了西边。
四处遍地都是梅树，枝叶茂密地连天空都被遮住了，很难看清周围的情况。
LONG
			);
		}
		else if (x == me->query_temp("plum_maze/south_exit") && y == MAZE_HEIGHT - 1) {
			can_s = 1;
			exit_num++;
			set("long", @LONG
这是梅林中的一条小路，小路往南就出了梅林，远处隐约现出白墙，似乎
是一个大庄院。四处遍地都是梅树，枝叶茂密地连天空都被遮住了，很难看清
周围的情况。
LONG
			);
		}
		else if (exit_num > 1){
			set("long", @LONG
这是梅林中的一条小路。举目望去，四处遍地都是梅树，枝叶茂密地连天
空都被遮住了，很难看清周围的情况。
LONG
			);
		}
		else{
			set("long", @LONG
这是梅林中的一条小路，你已经走到了小路的尽头，只有回头了。四处遍
地都是梅树，枝叶茂密地连天空都被遮住了，很难看清周围的情况。
LONG
			);
		}
		str = sprintf( "%s - %s\n    %s%s",
			env->query("short"),
			wizardp(me)? file_name(env): "",
			env->query("long"),
			env->query("outdoors")? NATURE_D->outdoor_room_description() : "" );

		if (exit_num == 1) {
			if (can_n)
				str += "    这里唯一的出口是 " + BOLD + "north" + NOR + "。\n";
			else if (can_s)
				str += "    这里唯一的出口是 " + BOLD + "south" + NOR + "。\n";
			else if (can_w)
				str += "    这里唯一的出口是 " + BOLD + "west" + NOR + "。\n";
			else if (can_e)
				str += "    这里唯一的出口是 " + BOLD + "east" + NOR + "。\n";
		}
		else {
			str += "    这里明显的出口是 " + BOLD;
			if (can_n) {
				str += "north";
				exit_num--;
				if (exit_num > 1)
					str += "、";
				else
					str += NOR + " 和 " + BOLD;
			}
			if (can_w) {
				str += "west";
				exit_num--;
				if (exit_num > 1)
					str += "、";
				else if (exit_num)
					str += NOR + " 和 " + BOLD;
				else
					str += NOR + "。\n";
			}
			if (can_e) {
				str += "east";
				exit_num--;
				if (exit_num)
					str += NOR + " 和 " + BOLD;
				else
					str += NOR + "。\n";
			}
			if (can_s) {
				str += "south";
				str += NOR + "。\n";
			}
		}

		inv = all_inventory(env);
		for (i = 0; i < sizeof(inv); i++) {
			if (inv[i] == me) continue;
			if (!me->visible(inv[i])) continue;
			str += "  " + inv[i]->short() + "\n";
		}

		write(str);
		return 1;
	}
	if (arg == "north") {
		if (x == me->query_temp("plum_maze/north_exit") && y == 0) {
			can_n = 1;
			set("exits/north", __DIR__"road2");
			return 0;
		}
		if (can_n) write("梅花掩映，你看不清那边的情况！\n");
		else write("你要看什么？\n");
		return 1;
	}
	if (arg == "south") {
		if (x == me->query_temp("plum_maze/south_exit") && y == MAZE_HEIGHT - 1) {
			can_s = 1;
			set("exits/south", __DIR__"road3");
			return 0;
		}
		if (can_s) write("梅花掩映，你看不清那边的情况！\n");
		else write("你要看什么？\n");
		return 1;
	}
	if (arg == "west") {
		if (can_w) write("梅花掩映，你看不清那边的情况！\n");
		else write("你要看什么？\n");
		return 1;
	}
	if (arg == "east") {
		if (can_e) write("梅花掩映，你看不清那边的情况！\n");
		else write("你要看什么？\n");
		return 1;
	}
	return 0;
}

int valid_leave(object me, string dir)
{
	object ob;
    int x, y, can_n, can_e, can_w, can_s, path;
	string min;

	if (objectp(ob = me->query_temp("follow"))) {
		me->set_leader(0);
		tell_object(me, ob->name() + "在梅林里左转右转，一眨眼就不见了。\n");
		return 0;
	}
	if (me->query("env/brief")) {
		me->delete("env/brief");
		tell_object(me, "这片梅林幽深曲折，你决定取消行走时的简短描述模式。\n");
	}
	x = me->query_temp("plum_maze/x") - 1;
	y = me->query_temp("plum_maze/y") - 1;
	path = me->query_temp("plum_maze/" + x + "_" + y);
	can_n = path & LINK_UP;
	can_w = path & LINK_LEFT;
	can_e = path & LINK_RIGHT;
	can_s = path & LINK_DOWN;

	if (dir == "north") {
		if (x == me->query_temp("plum_maze/north_exit") && y == 0) {
			me->delete_temp("plum_maze");
			me->move(__DIR__"road2");
			min = "";
			if (me->is_fighting())
				min += "从梅林跌跌撞撞地跑了过来，模样有些狼狈。\n";
			else {
				min += me->query_condition("killer") ? "神色慌张地" : "";
				min += "从梅林快步走了过来。\n";
			}
			tell_room(environment(me), me->name() + min, ({me}));
			return notify_fail("");
		}
		if (can_n) {
			me->add_temp("plum_maze/y", -1);
			return 1;
		}
		return notify_fail("这个方向没有出路。\n");
	}
	if (dir == "south") {
		if (x == me->query_temp("plum_maze/south_exit") && y == MAZE_HEIGHT - 1) {
			me->delete_temp("plum_maze");
			me->move(__DIR__"road3");
			min = "";
			if (me->is_fighting())
				min += "从梅林跌跌撞撞地跑了过来，模样有些狼狈。\n";
			else {
				min += me->query_condition("killer") ? "神色慌张地" : "";
				min += "从梅林快步走了过来。\n";
			}
			tell_room(environment(me), me->name() + min, ({me}));
			return notify_fail("");
		}
		if (can_s) {
			me->add_temp("plum_maze/y", 1);
			return 1;
		}
		return notify_fail("这个方向没有出路。\n");
	}
	if (dir == "west" && can_w) {
		me->add_temp("plum_maze/x", -1);
		return 1;
	}
	if (dir == "east" && can_e) {
		me->add_temp("plum_maze/x", 1);
		return 1;
	}
	return notify_fail("这个方向没有出路。\n");
}
