#include <ansi.h>

inherit ROOM;
string look_leitai();

void create()
{
	set("short", HIY"天地门"NOR);
	set("long", "这里是"+MUD_NAME+"里的天地门比武擂台，台下挤满了来自各门各派的观众。
围绕着擂台下(leitai)报名台(info)有无数高手，此处人声鼎沸，热闹无比，
这里通往东南西北各处比武看台。\n");

	set("no_fight", 1);
	set("no_sleep_room", 1);
	set("sleep_room", 1);
	set("leitai", "/cmds/leitai/leitai");

	set("exits", ([
		"out" : "/d/xiangyang/guangc",
		"east" : __DIR__"lta",
		"west" : __DIR__"ltb",
		"south" : __DIR__"ltc",
		"north" : __DIR__"ltd",
	]));

	set("item_desc",([
		"leitai" : (: look_leitai :),
		"info" : "\n您如要打擂，请输入：dalei\n",
	]));

	set("outdoors", "比武擂台");
	setup();
}

void init()
{
	object me = this_player();

	if (userp(me) && !wizardp(me))
		add_temp("num", 1);
	add_action("do_biwu", "dalei");
	add_action("do_action", ({"dazuo", "tuna", "lian", "learn", "du", "steal", "hit",
		"kill", "fight", "study", "practice","exercise", "exert"}));
}

#include "leitai_fun.h"
