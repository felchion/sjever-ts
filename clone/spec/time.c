//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (BLU"时间瓶"NOR, ({"time bottle","shijian ping"}));
	set("long","这是一个神秘的"HIM"时间瓶"NOR"，据说使用(use)它能找回曾经遗失的时间。\n");

	set("unit","个");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/time");
	set("rest",1);
	set("desc","增加二十四小时有效在线时间。");
	set("credit",10);	
	setup();
}


int restrict() {
	object me = this_player();

	if (!(me->query("mud_time") - me->query("eff_mud_time") > 0))
		return notify_fail("你没有遗失的时间。\n");
	
	return  ::restrict();
}

void init()
{
	add_action("do_use","use");
  
}

int do_use(string arg)
{
	object me=this_player();

	if (!arg)
		return notify_fail("你要使用什么?\n");

	if (arg!="ping" && arg!="bottle")
		return notify_fail("你要使用什么?\n");

	if (!(me->query("mud_time") - me->query("eff_mud_time") > 0))
		return notify_fail("你没有遗失的时间!\n");

	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}

	if (me->query("mud_time") - me->query("eff_mud_time") > 86400)
		me->add("eff_mud_time",86400);
	else
		me->set("eff_mud_time",me->query("mud_time"));
		
	write(HIW"你打开"BLU"时间瓶"HIW"，一缕奇异的流光把你笼罩在其中。\n"NOR);
	degree();
	return 1;
}