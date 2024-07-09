// Room: /d/city/laofang.c
// Build by Looklove@sj 2000/10/16
// 2001.9.29 update

#include <ansi.h>
#include <room.h>

inherit ROOM;

void create()
{
	set("short", "牢房");
	set("long", @LONG
这里是牢房，房间里很潮湿，光线昏暗，空气弥漫着一股臭气，让人浑身
都不舒服。几个在街上到处裸奔的囚犯躺在角落里呻吟着。透过冰冷厚重的铁
门，偶尔传来一阵阵的吆喝声。大概又是在提审什么犯人。
LONG
	);

	set("no_fight", 1);
	set("no_sleep_room", 1);
	set("no_update", 1);
	set("valid_startroom", 1);
	set("objects", ([
		__DIR__"npc/yuzu2" : 1,
	]));
	setup();
}

void init()
{
	add_action("do_dig", "dig");
}

int do_kill(string arg)
{
	string action = query_verb();
	switch (action) {
		case "kill":
		case "hit":
		case "throw":
		case "fight":
		case "perform":
		case "dazuo":
		case "exercise":
		case "tuna":
		case "lian":
		case "practice":
		case "du":
		case "study":
		case "respirate":
		write("都已经到这个地方了，还想那些俗事做什么？\n");
		return 1;
	}
	return 0;
}

int do_dig(string arg)
{
	object me=this_player();
	object money;

	if (me->is_busy()) {
		write("你正忙着呢\n");
		return 1;
	}
	   
	me->start_busy(1);
	if (!random(30)) {
		money=new("/clone/money/silver");
		message_vision("$N在地上一阵乱挖。\n",me);
		message_vision("$N忽然发现土里有一两银子。\n",me);
		money->set_amount(1);
		money->move(me);
		return 1;
	}
	else{
		message_vision("$N在地上一阵乱挖。\n",me);
		return 1;
	}
}
