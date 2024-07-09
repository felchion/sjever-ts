// By River@sj 99.5.24

#include <ansi.h>
#include <room.h>
inherit ROOM;

void create()
{
	set("short", YEL"山路"NOR);
	set("long", @LONG
这是一条崎岖的山路，路面上夹杂着一些泥沙。齐膝的野草几乎把路给掩
盖了，开出来的野花飘着阵阵花香。北面是一片树林，不时传来一阵阵的野兽
叫声, 头顶有一些不知名的小鸟在跳来跳去。
LONG
    );

	set("outdoors", "襄阳");

	set("exits", ([
		"north" : __DIR__"xiaolu1",
		"southeast" : "/d/xiangyang/conglin4",
	]));
	setup();
}

int valid_leave(object me, string dir)
{
	object *inv;

	if (dir == "north") {
		write("北面是一片林海，不时传来一阵阵的野兽叫声, \n");
                inv = filter_array(deep_inventory(me), (: userp :));
		if (me->query("ygpass") && ! sizeof(inv))
			write("由于你已经听了神雕侠杨过的指导，所以不用多想，信步就走出了林海。\n");
		else {
			tell_room(environment(me), me->name()+"往北面的"CYN"树林"NOR"快步离开。\n"NOR, ({ me }));
			me->move(__DIR__"linhai" + (random(10) + 1));
			tell_room(environment(me), me->name()+"从"YEL"山路"NOR"快步走了过来。\n"NOR, ({ me }));
			return notify_fail("");
		}
	}
	return ::valid_leave(me, dir);
}
