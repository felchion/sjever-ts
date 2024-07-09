//Room: songshan/guanshengfeng.c 观胜峰
//Modify by baimo for ss newbie job

#include <ansi.h>
inherit ROOM;
string look_sl();

string look_sl()
{
	object me, room;
	me = this_player();
	if (!(room = find_object("/d/shaolin/guangchang")))
		room = load_object("/d/shaolin/guangchang");
	if (!room || random(me->query_kar())<0)
		return "你努力向南边的少室山望去，却被茫茫云海所遮，看不清楚。\n";
	message_vision("$N站在峰上，回首向南面的少室山望去。\n", me);
	"/cmds/std/look.c"->look_room(this_player(), room);
	return "";
}

void create()
{
        set("short", "观胜峰");
        set("long", @LONG
站在这里，凌于瀑布之上，景色又是一番不同，处在一片山从之中，西北
是绝顶，西面是嵩山的各个山峰，南面遥遥相望的便是地处少室山的嵩山少林
寺(shaolin)，身处山中而不见归路。路边到处可见一堆堆的乱石。
LONG);
	set("exits", ([
		"northdown" : __DIR__"shanlu3",
		"eastdown" : __DIR__"gsfxia",
	]));
	set("item_desc",([
		"shaolin" : (: look_sl :),
	]));
	set("objects", ([
		CLASS_D("songshan") + "/sha" : 1,
	]));
	set("outdoors", "嵩山");
	setup();
}

void init()
{
	add_action("do_caishi", "ban");
}

int do_caishi(string arg)
{
	object me,ob;

	me = this_player();

	if (!me->query_temp("ss_qs/asked")) return notify_fail ("你要干嘛？\n");

	if (me->is_busy()) return notify_fail ("你现在正忙着呢！\n");

	if (me->is_fighting()) return notify_fail("你正在战斗中，无法专心干活！\n");

	if (!arg || arg!= "shikuai" ) return notify_fail ("你要干什么？\n");

	if ( me->query_temp("ss_qs/taked")
	 || (int)me->query("jingli") < 40
	 || (int)me->query("qi") < 30){
		message_vision("$N用力过猛，一头撞到石头上。\n",me);
		me->unconcious();
		return 1;
	}

	if (random(10) < 7 ){
		me->receive_damage("jingli",random(me->query("con"))/2);
		me->receive_damage("qi", random(me->query("str"))/2);
		message_vision("$N使出吃奶的劲想搬起一块石头，结果力气不够一屁股坐在地上。\n",me);
		return 1;
	}

	me->receive_damage("jingli",me->query("con")/2);
	me->receive_damage("qi",me->query("str")/2);

	message_vision("$N吃力地搬起一块大石头。\n",me);
	ob = new("/d/huanghe/npc/obj/shikuai");
	ob->move(me);
	me->set_temp("ss_qs/taked",1);

	if ((int)me->query_skill("hand", 1)< 51){
		write(HIY"你在搬石头的奔走之中，似乎对基本手法有些领悟。\n"NOR);
		me->improve_skill("hand", (int)(me->query("int")/2));
	}
	if ((int)me->query_skill("dodge", 1)< 31){
		write(HIY"你在搬石头的奔走中，似乎对基本轻功有些领悟。\n"NOR);
		me->improve_skill("dodge", (int)(me->query("int")/2));
	}
	return 1;
}
