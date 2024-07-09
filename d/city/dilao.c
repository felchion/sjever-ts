// Room: /d/city/dilao.c
// Build by Looklove@sj 2000/10/16
// 2001.9.29 update

#include <ansi.h>
#include <room.h>
inherit ROOM;

void create()
{
	set("short", "地牢");
	set("long", @LONG
这里是衙门后院的地牢，四处有几个看守在巡逻，气氛森严。不时有来提
审的衙役隔着铁门向里面大声吆喝着囚犯的名字。偶尔有几个犯人在看守的押
送下，送来这里关押。西边的铁门上开有一个小窗口(window)。为了方便探
监的百姓，可以从这个窗口递(pass)些吃的东西进去。
LONG
 );

	set("item_desc", ([
		"men" : "这是一扇极厚的铁门。\n",
	]));

	set("exits", ([
		"southup" : __DIR__"fyhouyuan",
	]));

	set("objects", ([
		__DIR__"npc/yayi" : 1,
	]));
	set("incity",1);
	setup();
}
void init()
{
	add_action("do_visit", "look");
	//       add_action("do_pass", "pass");
	add_action("do_unlock", "unlock");
	add_action("do_unlock", "kai");
}

int do_pass(string arg)
{
	object ob, me = this_player();

	if (!arg || !(ob = present(arg, this_player())))
	return notify_fail("你要送什么？\n");

	if (ob->query_weight() > 5000)
	return notify_fail(ob->query("name")+"太大了，不能从窗口里塞进去。\n");

	if (ob->query("no_drop") || ob->query("id") == "man tou" || ob->query("id") == "hulu" || ob->is_container() || ob->query("hsf"))
	return notify_fail("这个东西放不进去。\n");

	message_vision("$N把一"+ob->query("unit")+"$n从窗口塞了进去。\n", me, ob);
	ob->move("/d/city/laofang");
	message("vision", me->name()+"从窗外塞进来一"
	+ob->query("unit")+ob->query("name")+"。\n",environment(ob), ob);
	return 1;
}
int do_visit(string arg)
{
	object env, me = this_player();

	if (arg != "window") return 0;

	if(!( env = find_object("/d/city/laofang2")) )
	env = load_object("/d/city/laofang2");

	message_vision("$N趴在窗台上往里望去。\n", me);
	tell_room(env, "你感到有一双眼睛在窗外打量你。\n");
	return COMMAND_DIR"std/look"->look_room(me, env);
}
int do_unlock(string arg)
{
	object ob,me;
	me = this_player();

	if (query("exits/west"))
	return notify_fail("这扇门已经是打开的。\n");
	if (!arg || (arg != "men" && arg != "west"))
	return notify_fail("你要打开什么？\n");
	if (!(ob = present("dilao yaoshi", me)))
	return notify_fail("你没有钥匙，打不开这个牢门。\n");

	set("exits/west", __DIR__"laofang");
	me->set("succor",1);
	message_vision("$N拿出一把钥匙打开了牢门，可是钥匙却卡在门上拿不下来了。\n", me);
	destruct(ob);
	remove_call_out("do_close");
	call_out("do_close", 20, me);
	return 1;
}
int valid_leave(object me, string dir)
{
	if (!wizardp(me) && objectp(present("kan shou", environment(me))) &&
	dir == "west")
	return notify_fail("看守拦住了你的去路。\n");
	return ::valid_leave(me, dir);
}
void do_close(object me)
{
	if (me) message_vision(HIB"牢房的铁门发出一阵响声，自己关上了。\n"NOR, me);
	delete("exits/west");
}