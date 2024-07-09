
inherit ITEM;
#include <ansi.h>
string do_look();

void create()
{
	set_name("菜谱", ({ "cai pu", "pu", "hong's menu" }));
	set_weight(150);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "本");
		set("long", (: do_look:));
		set("value", 100);
		set("no_get", 1);
		set("no_give", 1);
		set("no_steal", 1);
		set("no_drop", 1);
	}
	setup();
}

void init()
{
	add_action("do_make", ({"make", "zuo"}));
}

int do_make(string arg)
{
	object me, ob, meat1, meat2, meat3, meat4, meat5;
	int i ;	

	me = this_player();
	meat1 = present("yanggao zuotun", me);
	meat2 = present("xiaozhu erduo", me);
	meat3 = present("xiaoniu yaozi", me);
	meat4 = present("zhangtui rou", me);
	meat5 = present("tu rou", me);

	if ( ! arg || arg != "cai")
		return 0;

	if ( ! me->query_temp("potjob/job", 1))
		return 0;

	if ( me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢！\n");

	if ( ! meat1 || ! meat2 || !meat3 || !meat4 || !meat5)
		return notify_fail("好像还缺少一些原料啊？\n");

	if ( meat1->query_temp("job_id") != me )
		return notify_fail("你的羊羔坐臀好像不是洪七公要你找的那种原料吧？\n");

	if ( meat2->query_temp("job_id") != me )
		return notify_fail("你的小猪耳朵好像不是洪七公要你找的那种原料吧？\n");

	if ( meat3->query_temp("job_id") != me )
		return notify_fail("你的小牛腰子好像不是洪七公要你找的那种原料吧？\n");

	if ( meat4->query_temp("job_id") != me )
		return notify_fail("你的獐腿肉好像不是洪七公要你找的那种原料吧？\n");

	if ( meat5->query_temp("job_id") != me )
		return notify_fail("你的免肉好像不是洪七公要你找的那种原料吧？\n");

	destruct(meat1);
	destruct(meat2);
	destruct(meat3);
	destruct(meat4);
	destruct(meat5);

	i = me->query_temp("potjob/do_time") +1;
	i *= 3;

	if ( random(i)){
		ob = new(__DIR__"pot_food");
		ob->set_name("玉笛谁家听落梅", ({"yudi luomei", "luomei", "food"}));
		ob->set("long", "这是一盘「玉笛谁家听落梅」，香飘四溢，看起来味道不错。\n");
		ob->set("unit", "盘");
		ob->setup();
		ob->move(me);
		me->set_temp("potjob/finish", 1);
		message_vision(HIY"$N将原料放在一起，一会就做出了一盘香口美味的「玉笛谁家听落梅」。\n"NOR, me);
	}
	else {
		me->delete_temp("potjob");
		message_vision(HIY"$N一时走神，放错了配料，浪费了制成「玉笛谁家听落梅」的大好机会。\n"NOR, me);
	}
	return 1;
}

string do_look()
{
	object me = this_player();
	mapping place, be_get;
	int i, j;
	string msg;

	if ( me->query_temp("potjob/job")) {
		if ( me->query_temp("potjob/finish"))
			return "你已经顺利完成了「玉笛谁家听落梅」的料理。\n";

		place = me->query_temp("potjob/pot_job_place");
		i = sizeof(keys(place));
		if ( i > 0) {
			msg = "你还有下列原料尚未找到：\n\n";
			for( j =0; j < i; j ++){
				msg += sprintf("%-8s  %-20s  %-8s\n", keys(place)[j], values(place)[j], 
				me->query_temp("potjob/for_menu/"+keys(place)[j]));
			}
			msg += "\n你已经找到的原料有：";
			be_get = me->query_temp("potjob/be_get");
			if ( be_get )
				i = sizeof(keys(be_get));
			else 
				i = 0;
			if ( i > 0 ){
				for (j = 0; j < i; j ++){
					msg += sprintf("%s ", keys(be_get)[j]);
				}
			}
			else
				msg += "无";
			msg += "\n";
			return msg;
		}
		return "你已经找齐了原料，赶快做「玉笛谁家听落梅」(zuo cai)吧。\n";
	}
	return "一本菜谱，看来是教你如何烹饪「玉笛谁家听落梅」的书。\n";
}
