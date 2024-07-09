// Room: /d/tianshan/chtbu.c
// By Linux

#include <ansi.h>
inherit ROOM;
string look_deng1();
string look_deng2();
string look_deng3();
string look_deng4();

void create()
{
	set("short", "成天部");
	set("long", @LONG
眼前是一个可容纳数百人的大厅，是成天部的所在地，大厅墙壁上画满了
浮雕，天顶上画的是一个青衫男子，头上有角，左手持长剑，右手握剑诀，骑
着一只青龙，冲天而起。地面以青色岩石磨就，分毫无缝，浑然天成。大厅四
角点着数盏明灯，弥漫在大厅里的香气好象就是从灯里发出。
LONG);

	set("exits", ([
	"eastdown" : __DIR__"dadao9",
	]));

	set("objects", ([
	__DIR__"npc/f-dizi" : 1,
	]));

	set("item_desc", ([
	"east deng" : (: look_deng1 :),
	"south deng" : (: look_deng2 :),
	"west deng" : (: look_deng3 :),
	"north deng" : (: look_deng4 :),
	]));
	set("zhbfang", 1);
	setup();
}

void init()
{
	add_action("do_smell", ({"smell", "wen"}));
	add_action("do_move", ({"move", "yi"}));
}

void reset()
{
	set("zhbfang", 1);
}

string look_deng1()
{
	object me = this_player();
	if (!me->query_temp("ts/smell")) return "什么？\n";
	if(random(me->query("kar")+me->query("per")+me->query("pur")) <= 40){
		me->delete_temp("ts/smell");
		return "你仔细看着点燃在大厅东角的铜灯，发现没有什么两样。\n";
	}
	else {
		me->set_temp("ts/smell2",1);
		return "你仔细看着点燃在大厅东角的铜灯，终于发现其中一盏的灯座，似乎可以移动。\n";
	}
}

string look_deng2()
{
	object me = this_player();
	if (!me->query_temp("ts/smell")) return "什么？\n";
	if(random(me->query("kar")+me->query("per")+me->query("pur")) <= 40){
		me->delete_temp("ts/smell");
		return "你仔细看着点燃在大厅南角的铜灯，发现没有什么两样。\n";
	}
	else {
		me->set_temp("ts/smell2",1);
		return "你仔细看着点燃在大厅南角的铜灯，终于发现其中一盏的灯座，似乎可以移动。\n";
	}
}

string look_deng3()
{
	object me = this_player();
	if (!me->query_temp("ts/smell")) return "什么？\n";
	if(random(me->query("kar")+me->query("per")+me->query("pur")) <= 40){
		me->delete_temp("ts/smell");
		return "你仔细看着点燃在大厅西角的铜灯，发现没有什么两样。\n";
	}
	else {
		me->set_temp("ts/smell2",1);
		return "你仔细看着点燃在大厅西角的铜灯，终于发现其中一盏的灯座，似乎可以移动。\n";
	}
}

string look_deng4()
{
	object me = this_player();
	if (!me->query_temp("ts/smell")) return "什么？\n";
	if(random(me->query("kar")+me->query("per")+me->query("pur")) <= 40){
		me->delete_temp("ts/smell");
		return "你仔细看着点燃在大厅北角的铜灯，发现没有什么两样。\n";
	}
	else {
		me->set_temp("ts/smell2",1);
		return "你仔细看着点燃在大厅北角的铜灯，终于发现其中一盏的灯座，似乎可以移动。\n";
	}
}

int do_smell(string arg)
{
	object me=this_player();
	if (me->is_busy() || me->is_fighting())
	return notify_fail("你正忙着呢！\n");
	tell_object(me,"你仔细闻着空气里的香气，似乎和其他几个大厅的气味不同，不由的暗自起疑!\n");
	me->set_temp("ts/smell",1);
	me->add_busy(2);
	return 1;
}

int do_move(string arg)
{
	object ob, *ob2;
	object here = this_object();
	object me = this_player();
	int i;

	if (me->is_busy() || me->is_fighting())	return notify_fail("你正忙着呢！\n");
	if (!me->query_temp("ts/smell2"))
	return notify_fail("什么？\n");
	if( (!arg) || !((arg == "deng") || (arg == "灯") || (arg == "tong deng") || (arg == "铜灯")))
	return notify_fail("你要移动什么？\n");
	if (query("zhbfang") < 1)
	return notify_fail ("你把铜灯往旁边移了移，但没发现任何东西。\n");

	ob=new(__DIR__"npc/zhbfan");
	ob->move(environment(me));
	message_vision(HIW"$N把铜灯往旁边移了移。\n"NOR, me);
	message_vision(HIW"突然一条人影向$N扑了过来。\n"NOR, me);
	message_vision(HIR"卓不凡对$N大喝一声：既然我上的山来，就是要和你们这些所谓仙子，拼个你死我活，\n"+
	"既然被你个" + RANK_D->query_rude(me) + "发现了，不是你死就是我活。\n"NOR, me);
	here->add("zhbfang",-1);
	ob2 = all_inventory(here);
	for(i=0; i<sizeof(ob2); i++) {
		if(! ob2[i]->is_character()) continue;
		if( ob2[i]->is_corpse()) continue;
		ob->kill_ob(ob2[i]);
		ob2[i]->kill_ob(ob);
	}
	me->delete_temp("ts/smell1");
	me->delete_temp("ts/smell2");
	return 1;
}

int valid_leave(object me,string dir)
{
	me->delete_temp("ts/smell");
	me->delete_temp("ts/smell2");
	return ::valid_leave(me,dir);
}
