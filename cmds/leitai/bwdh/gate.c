//试剑山庄  gate.c

#include <ansi.h>
#include <room.h>

inherit ROOM;
void create()
{
	set("short",HIY"庄院"NOR);
	set("long", @LONG
眼面的庄院就是名满天下的试剑山庄了，你不觉松了一口气，前往山庄的地
上打扫的一尘不染，汉白玉砌成的台阶豪华典雅，古红木定制的大门紧闭着，使
人无法洞悉门内的一切，三丈多高的琉璃墙在透过树叶的阳光照耀下闪闪发亮，
寂静的四周只闻的远方鸟儿的鸣嘀声。
LONG
	);
        set("exits", ([
		"north" : __DIR__"qianting",
		"out": __DIR__"zongtai",
	]));

	set("no_dig", 1);
	set("outdoors", "试剑山庄");
	setup();
}

int valid_leave(object me, string dir)
{
	object *ob = deep_inventory(me);
	int i = sizeof(ob);
	if( dir == "out") {
		while (i--)
		if( ob[i]->query("secret_obj"))
			return notify_fail("你带着这么贵重的东西，想离开试剑山庄是不可能的。\n");
	}
	return ::valid_leave(me, dir);
}
