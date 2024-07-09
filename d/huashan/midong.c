// Room: /d/huashan/midong.c
// Date: Look 99/03/25

#include <room.h>
inherit ROOM;

void create()
{
       set("short","秘洞");
       set("long", @LONG
洞里漆黑一团，透过从洞口远远透过来的一丝亮光你发现四周的洞壁上似
乎刻着一些小人，但怎么也看不清楚。
LONG
     );

	set("exits", ([
		"northeast" : __FILE__,
		"southeast" : __FILE__,
		"southwest" : __FILE__,
		"northwest" : __FILE__,
		"west" : __FILE__,
		"east" : __FILE__,
		"south" : __FILE__,
		"north" : __FILE__,
	]));
	setup();
}

void init()
{
	object ob;
	int jing, jing_cost;

	ob = this_player();
	jing = ob->query("jing");
	jing_cost = random(30);

	ob->add("jing", -jing_cost);
	if(jing < jing_cost + 50)
		ob->unconcious();
}

int valid_leave(object me, string dir)
{
	if (random(60) > 14)
		me->add_temp("mark/steps",1);
	if (random(20) < 10)
		me->add_temp("mark/steps",-1); 

	if (me->query_temp("mark/steps") == 10 ){
		if(random(4)) {
			me->move(__DIR__"houshan");
			me->delete_temp("mark/steps");
			return notify_fail("你瞎打瞎撞，忽然发现眼前一亮，已经来到了人迹罕至的后山。\n");
		}
		else  {
			me->move(__DIR__"siguoya");
			me->delete_temp("mark/steps");
			return notify_fail("你累得半死，终於走出了山洞。\n");
		}
	}
	
	return ::valid_leave(me, dir);
}
