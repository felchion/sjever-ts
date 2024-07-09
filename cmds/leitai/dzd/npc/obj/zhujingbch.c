// /d/dzd/obj/xsycw.c

#include <ansi.h>

inherit ITEM;

void create()
{
	set_name("朱睛冰蟾",({"zhujing bingchan","bingchan"}));
	set_weight(100);
	if(clonep())
		set_default_object(__FILE__);
	else {
		set("long","这是产于西域雪山的名贵药材，通体雪白，两只眼珠却血也般红。
不但能吃，还能吸毒(xidu)哦。\n");
		set("value",10000);
		set("unit","只");
		//set("no_drop",1);
	}
	setup();
}

void init()
{
	add_action("do_eat","eat");
	add_action("do_eat","fu");
	add_action("do_eat","chi");
	add_action("do_xidu","xidu");
}

int do_eat(string arg)
{
	object me = this_player();	
	if(!arg || arg != "bingchan")
		return notify_fail("你要吃什么？\n");
	if(me->query("qi")==me->query("max_qi") && me->query("jing")==me->query("max_jing"))
		return notify_fail("你没事吃这个做什么？\n");		
	message_vision("$N吃下一只" + name() + ",伤势顿时全好！\n",me);
	me->set("eff_qi",me->query("max_qi"));
	me->set("qi",me->query("max_qi"));
	me->set("eff_jing",me->query("max_jing"));
	me->set("jing",me->query("max_jing"));
	destruct(this_object());
	return 1;
}	

int do_xidu(string arg)
{
	object ob;
	object me = this_player();
	if(!arg) ob = this_player();
	else 
		{
			if(!objectp(ob = present(arg, environment(me))))
				return notify_fail("你要给谁吸毒？\n");
			if (!ob->is_character() || ob->is_corpse())
				return notify_fail("看清楚一点，那并不是活物。\n");
		}
	if( ob->is_fighting() )
		return notify_fail("你无法在战斗中运功吸毒。\n");
		
	if( !mapp(ob->query_conditions_by_type("poison"))  )
	  return notify_fail(ob->query("name")+"并没有中毒。\n");
	if(me==ob)
		message_vision(HIW"$N将冰蟾放在伤口上，冰蟾嘴对着伤口，不一会，通体雪白的冰蟾渐渐变得灰黑,$N体内的毒已被清除！\n"NOR,me);
	else
		message_vision(HIW"$N将冰蟾放在$n伤口上，冰蟾嘴对着伤口，不一会，通体雪白的冰蝉渐渐变得灰黑,$n体内的毒已被清除！\n"NOR,me,ob);
		ob->clear_conditions_by_type("poison");
		me->start_busy(1);
		ob->start_busy(2);
		return 1;
}