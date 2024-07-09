//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (HIB"妖异葡萄"NOR, ({ "xputao"}));
	set("long","这是一颗"HIB"妖异葡萄"NOR"，吃(eat)下它会发生奇怪的事情。\n");

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/xputao");
	set("desc","扣除一万点经验，增加潜能二千点。");
	set("credit",10);
	setup();
}

void init()
{
	add_action("do_eat","eat");  
}

int restrict() {
	object me = this_player();

	if (me->query("combat_exp") < 100000) 
    return notify_fail("你还是多努力一段时间吧。\n");

	return  ::restrict();
}

int do_eat(string arg)
{
	object me=this_player();


	if (arg!="putao" && arg!="xputao")
	return notify_fail("你要吃什么?\n");

	if (!restrict()) {return notify_fail("你现在不能吃"+this_object()->query("name")+"了。\n");}

	me->add("combat_exp",-10000);
	me->add("potential",2000);
	write(HIY"你吃下一颗"HIB"妖异葡萄"HIY"发现自己修为大损、潜能大增!\n"NOR);
	degree();
	return 1;
}