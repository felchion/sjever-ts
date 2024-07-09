//flowray 2008.1.12
#include <ansi.h>

inherit SPEC; 

void create()
{
	set_name (HIG"贵宾月卡"NOR, ({ "vm card" , "card" }));
	set("long","这是一张"HIG"书剑贵宾卡"NOR"，使用(use)后可以增加三十天贵宾时间。\n");

	set("unit","张");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/sjvip");
	set("desc","增加三十天贵宾时间。");
//	set("rest",1);
	set("credit",100);	
	setup();
}

void init()
{
	add_action("do_use","use");
  
}


int do_use(string arg)
{
	object me=this_player();
	int vip_time = 30 * 86400;
	if (!arg)
	return notify_fail("你要用什么?\n");

	if (arg!="card")
	return notify_fail("你要用什么?\n");

	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}

	if((int)me->query("vip/vip_time")!=0 && (int)me->query("vip/vip_time") < time())
	{
		me->delete("vip/vip_time");
		me->delete("vip/vip_start_time");
	}

	if (!(int)me->query("vip/vip_start_time"))
	{
		me->set("vip/vip_time",vip_time + time());
		me->set("vip/vip_start_time",time());
		tell_object(me,"你使用"HIG"书剑贵宾卡"NOR"开通了三十天的贵宾，请使用 time 命令进行查看。\n");
	}
	else
	{
		me->add("vip/vip_time",vip_time);
		tell_object(me,"你使用"HIG"书剑贵宾卡"NOR"增加了三十天的贵宾，请使用 time 命令进行查看。\n");
	}
	
	if (me->query("vip/vip_total")<0)
		me->set("vip/vip_total",vip_time);
	else
		me->add("vip/vip_total",vip_time);
	
	log_file("static/VIP",sprintf("| %s(%s)使用VIP卡成功增加了三十天贵宾时间，过期时间(%d)。\n",me->query("name"),me->query("id"),me->query("vip/vip_time")));
	degree();
	return 1;
}