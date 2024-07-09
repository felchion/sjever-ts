// /d/dzd/obj/xsycw.c

#include <ansi.h>

inherit ITEM;

void create()
{
	set_name("雪参玉蟾丸",({"xueshen yuchanwan","wan"}));
	set_weight(100);
	if(clonep())
		set_default_object(__FILE__);
	else {
		set("long","这是一粒朝鲜国王进贡的疗伤圣药。色泽晶莹，通体半透明状,嗅之则有淡淡清香。\n");
		set("value",10000);
		set("unit","粒");
		set("no_drop",1);
	}
	setup();
}

void init()
{
	add_action("do_eat","eat");
	add_action("do_eat","fu");
	add_action("do_eat","chi");
}

int do_eat(string arg)
{
	object me = this_player();
	object ob = this_object();	
	if(!arg || ( arg!= "wan" && arg!="xueshen yuchanwan"))
		return 0;
	if(me->query("qi")==me->query("max_qi") && me->query("jing")==me->query("max_jing"))
		return notify_fail("你没事吃这个做什么？\n");
	message_vision("$N吃下一粒" + name() + ",伤势顿时好转！\n",me);
	me->set("eff_qi",me->query("max_qi"));
	me->set("qi",me->query("max_qi"));
	me->set("eff_jing",me->query("max_jing"));
	me->set("jing",me->query("max_jing"));
	ob->add_temp("used",1);
	if(ob->query_temp("used") > 3 )
		destruct(this_object());
	return 1;
}



