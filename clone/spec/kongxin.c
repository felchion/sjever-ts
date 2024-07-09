//flowray 2005.6.16
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (WHT"空心散"NOR, ({ "kongxin san","kongxin","san"}));
	set("degree",1);
	set("long","这是一包"WHT"空心散"NOR"，你可以服(fu)下它解除琐事的困扰。\n");

	set("unit","包");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	
	set("flag","spec/kongxin");
	set("desc","消除任务繁忙状态。");
	set("credit",0);	
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
	    
	if (arg!="kongxin" && arg!="san")
      		return notify_fail("你要服用什么?\n");
      	
      	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}
    
	if (!me->query_condition("job_busy") && !me->query("job_name"))
		return notify_fail("你现在不需要服用空心散。\n");
	
	me->delete("job_name");
	me->clear_condition("job_busy");
	write("你服下一些"WHT"空心散"NOR"觉得心宁气定,把江湖琐事忘得一干二净!\n");
	degree();
	return 1;
}	 