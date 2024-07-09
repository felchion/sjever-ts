// xiangcha.c 香茶
// By Marz 03/29/96
// LKLV@SJ 2001.9.28

inherit ITEM;

void create()
{
	set_name("香茶",({"xiang cha", "tea", "cha", "xiangcha"}));
	set_weight(50);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "一杯热茶，悠悠地冒着香气～～～\n");
		set("unit", "杯");
		set("value", 10);
		set("remaining", 2);
		set("drink_supply", 25);
	}

	setup();
}

void init()
{
	add_action("do_drink", "drink");
	add_action("do_drink", "he");
}

int do_drink(string arg)
{

        object ob, me;
        int recover, jing, e_jing, m_jing;

        me = this_player();
        ob = this_object();
	recover = 5;

	if( !ob->id(arg) ) return 0;
	if( me->is_busy() )
		return notify_fail("你上一个动作还没有完成。\n");
	if( me->query("water") >= me->max_water_capacity() )
		return notify_fail("你已经喝太多了，再也灌不下一滴水了。\n");

	set("value", 0);
	me->add("water", (int)query("drink_supply"));
	jing = (int)me->query("jing");
	e_jing = (int)me->query("eff_jing");
	m_jing = (int)me->query("max_jing");

	if (jing < e_jing ){
		if ( (jing + recover) >= e_jing ){
			me->set("jing", e_jing);
		} else {
			me->set("jing", jing+recover);
		}
	}

	if( me->is_fighting() ) me->start_busy(2);

	add("remaining", -1);
	if ( query("remaining") ){
		message_vision("$N端起杯香茶，有滋有味地品了几口。\n"+
		"一股香气直入心脾，$N觉得精神好多了。\n", me);
	} else {
		message_vision("$N端起雕花小杯，把剩下的香茶一饮而尽。\n"+
		"一股香气直入心脾，$N觉得精神好多了。\n", me);
		destruct(ob);
	}
	return 1;
}