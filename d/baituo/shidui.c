#include <ansi.h>
inherit ROOM;
void create()
{
	set("short", "乱石堆");
	set("long", @LONG
这里四周所围全是巨石，地上铺满了凌乱的碎石子，象是被人击打巨
石所打下来的碎块。正中一块平滑的青石板上蹲着一只雪白的蛤蟆。
LONG);
	set("objects", ([
		__DIR__"npc/chanchu" : 1,
		BINGQI_D("gs-zhang") : 1,
	]));
	set("item_desc", ([
		"stone": "\n这些巨石都有凹面，看得出是被人拿来练掌力用的。\n",
	]));
	set("outdoors", "白陀山");
	setup();
}

void init()
{
	object me = this_player();
	if(!me->query_temp("baituo_yun")
	&& me->query("id") != "xuanbing chanchu"
	&& !me->query("quest/hama/pass") ){
		message_vision(HIW"玄冰蟾蜍突然纵起，对着$N一口毒气喷出！\n"NOR,this_player());
		tell_object(me, HIB "你感到脑中一阵旋晕，立时昏了过去。\n" NOR );
		me->unconcious();
		me->move(__DIR__"yuanzi5");
		tell_room(environment(me), me->name()+"突然从巨石之上直落下来，‘叭’的一声摔在地上！\n", ({ me }));
		if(random(me->query_skill("poison", 1)) < 100 ) {
			me->set_temp("last_damage_from", "中剧毒而");
			me->die();
		}

		else me->add("max_neili", -5);
	}
	add_action("do_climb", "climb");
	add_action("do_climb", "pa");
	add_action("do_strike", "strike");
}

int do_climb(string arg)
{
	object me = this_player();
	if( !arg || arg=="" ) return 0;

	if( (int)me->is_busy() || me->is_fighting())
	return notify_fail("你正忙着呢。\n");

	if (arg == "stone" ){
		message_vision("$N手脚并用，爬上了巨石去，翻身一纵，已经跳了出去。\n", me);
		me->move(__DIR__"yuanzi5");
		message("vision", me->name() + "从里面跳了出来。\n",environment(me), ({me}) );
		me->start_busy(1);
		if( !me->query_skill("hamabu", 1) && me->query_skill("hamagong", 1) ){
			tell_object(me, "你回头看看蟾蜍跃起的姿势，领悟出了蛤蟆步法来！\n");
			me->set_skill("hamabu", 1);
		}

		return 1;
	}
	return 0;
}

int do_strike(string arg)
{
	int improve, cost;
	object me = this_player();

	improve = me->query_int() / 10 + random(me->query_int() * 5 / 6);
	improve /= 2;
	if( !arg || arg=="" ) return 0;

	if( (int)me->is_busy() || me->is_fighting())
	return notify_fail("你正忙着呢。\n");

	if (arg == "stone" ){
		if(!me->query("quest/hama/pass"))
		return notify_fail("你用手指推了推巨石，脸上露出一丝难堪的神色。\n");
		if( me->query_skill("hamagong", 1) > 64 && me->query("oyf/hamagong") < 2)
		return notify_fail("你用手指推了推巨石，脸上露出一丝难堪的神色。\n");
		if( me->query_skill("hamagong", 1) > 100 )
		return notify_fail("你一掌向巨石推去，只听轰的一声，巨石被你掌力震成两半！\n");

		if( me->query_skill("hamagong", 1) == 100 && (me->query("oyf/hmg_add") < 1) ) {
			message_vision(WHT"\n$N一掌向巨石推去，只听轰的一声，巨石被$P掌力震成两半！\n",me);
			tell_object(me, "你这一掌劲力十足，看来可以出关了！\n你信心十足，武艺提高了不少。\n");
			me->add("combat_exp", 30000);
			me->add("max_neili", 300);
			me->add("max_jingli", 200);
			me->set_skill("hamagong", 110);
			me->set_skill("force", me->query_skill("force", 1)+10);
			log_file("quest/oyf_log", sprintf("%s(%s) 于 %s 练习HMG至100级，并得到奖励。\n", me->name(1), me->query("id"), ctime(time()) ) );
			return 1;
		}

		cost = 600 / (int)me->query("int");
		cost = 600 / (int)me->query("int");

		if ((int)me->query("jingli") < cost || (int)me->query("jing") < cost){
			message_vision("$N用力过猛, 一不留神劲力反弹，震得$N晕了过去。\n",me);
			me->unconcious();
			return 1;
		}

		message_vision("$N默诵蛤蟆功口诀，运力于双掌之上，一掌掌向巨石推去。\n"NOR, me);
		me->improve_skill("hamagong", improve);
		me->improve_skill("strike", improve/2);
		me->receive_damage("jing", cost, "消耗过度");
		me->receive_damage("neili", cost/2, "消耗过度");
		me->receive_damage("jingli", cost, "消耗过度");
		return 1;
	}
	return 0;
}

int valid_leave(object me, string dir)
{
	me->delete_temp("baituo_yun");
	return ::valid_leave(me,dir);
}