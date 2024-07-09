// Room: /d/huashan/fanting.c
// Date: Look 99/03/25

inherit ROOM;

#include <ansi.h>

string* names = ({
	FOOD_D("baozi"),
	FOOD_D("cuishan"),
	FOOD_D("douhua"),
	FOOD_D("doufu"),
	FOOD_D("guoba"),
	FOOD_D("jitui"),
	FOOD_D("liji"),
	FOOD_D("liyu"),
	FOOD_D("niurou"),
	FOOD_D("peanut"),
//	FOOD_D("ruzhu"),
	FOOD_D("sansi"),
	FOOD_D("yaoliu"),
});

void create()
{
	set("short", "饭厅");
	set("long", @LONG
这里便是华山派弟子用饭的地方。饭厅不是很大，但窗明几净。桌上摆着
一些食物，让人垂涎欲滴。不时有一些华山弟子走来走去，想是在忙些什么。
LONG
	);

	set("exits", ([
		"north" : __DIR__"yaofang",
		"east" : __DIR__"celang3",
	]));
	set("objects",([
			__DIR__"npc/puren" : 1,
        	__DIR__"obj/qingshui-hulu" : 1,
	]));
	set("indoors", "华山" );
	setup();
}

int not_at_all(string arg)
{
	if (arg == "all") {
		write("仆人说道：这里地方本来就小，动作轻点。\n");
		return 1;
	}
	return 0;
}

void init()
{
	object obj1, obj2;
	object me = this_player();
	if (me->query("family/family_name") == "华山派" 
	&& (int)me->query("food") < 100
	&& present("pu ren", environment(me))
	&& random(10) < 8 ) {
	obj1 = new(FOOD_D("rice"));
        obj2 = new(names[random(sizeof(names))]);
        if( clonep(obj2) && clonep(obj1)) {
        	message_vision(HIC"\n仆人说道：饭菜已经准备好了，快趁热吃吧。\n" NOR, me);
        	obj1->move(me);
        	obj2->move(me);
		}
	}
	add_action("not_at_all", "drop");
}

int valid_leave(object me, string dir)
{
	if ( present("pu ren", environment(me))
    && (present("rice", me)
	  || present("fanqie yaoliu", me) || present("kousansi", me)
	  || present("niurou", me) || present("huasheng", me)
	  || present("tangcu liyu", me) || present("zhuachao liji", me)
	  || present("jitui", me) || present("rou pian", me)
	  || present("doufu", me) || present("douhua", me)
	  || present("liangxi cuishan", me) )
	  && ( dir == "east" || dir == "north" ) )
	{
		return notify_fail("仆人说道：就在这里趁热吃吧，带走干什么？\n");
	}
	return ::valid_leave(me, dir);
}
