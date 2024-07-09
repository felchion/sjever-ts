//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (RED"大还丹"NOR, ({ "dahuan dan","dahuan","dan"}));
	set("long","这是一颗"RED"大还丹"NOR"，据说有着几近起死回生的效果。\n");

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/dahuan");
	set("desc","武林圣药疗伤极品。");
	set("credit",2);	
	setup();
}

void init()
{
	add_action("do_eat","eat");
	add_action("do_eat","fu");
  
}

int do_eat(string arg)
{
	object me=this_player();
	    
	if (!arg)
      		return notify_fail("你要服用什么?\n");
      		
	if (arg!="dan" && arg!="danhuan" && arg!="danhuan dan")
      		return notify_fail("你要服用什么?\n");
      	
      		
      	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}   
	
	if (me->is_busy()) {return notify_fail("你正忙着呢。\n");}
			
	"/adm/daemons/emoted"->do_emote(me,"taste");
	me->reincarnate();
	me->set("neili", me->query("max_neili")*2);
	me->set("jingli", me->query("eff_jingli")*2);
		
	write(HIW"你吃下一颗"RED"大还丹"HIW"顿时伤势痊愈气血充盈。\n"NOR);
	degree();
	return 1;
}	 