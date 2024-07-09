// 会客室

#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", HIC"会客室"NOR);
        set("long", @LONG
这里是管理员的会客室，是用来接待玩家的地方。室内窗明几净，陈设简
单。楼上是一间休息室，管理员平常都是在那里休息。一般玩家到这里后就上
不去了。东面的出口通往扬州城隍庙。
    边上有台饮水机可以(drink),不过好象没有提供点心。
LONG);

        set("item_desc", ([
                "up": "你觉得眼前一片光明，什么也看不清楚。\n",
        ]));

        set("exits", ([
                "up": __DIR__"wizard_room",
		"southeast": "/d/city/chmiao",
	]));
        set("no_fight", 1);
	set("sleep_room", 1);
	set("no_sleep_room", 1);

	set("objects", ([
		//"/clone/npc/wizard" : 1,
		"/clone/misc/goldbox" : 1,
	]));

	set("incity",1);
	setup();
        call_other("/clone/board/towiz_b", "???");
}

int valid_leave(object me, string dir)
{
        if( dir=="up" && !wizardp(me))
                return notify_fail("那里现在不能进去。\n");
        return ::valid_leave(me, dir);
}

void init()
{
	add_action("do_he",({"he","drink"}) );
}

int do_he(string arg)
{
     object me = this_player();
	if (me->query("water") < me->query("str")*10+100){
         message_vision(HIC"$N打开饮水机，倒了一杯水，美滋滋得喝了一口，随即把一次性杯丢到窗外去了。\n"NOR, me);
         me->add("water", 50, me);
         return 1;
       }
       return notify_fail("你喝太多了，不怕撑死？\n");
}
