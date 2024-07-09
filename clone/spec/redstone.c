//flowray 2008.1.12
#include <ansi.h>

inherit SPEC; 

void create()
{
	set_name (HIR"赤晶石"NOR, ({ "redstone" , "stone" }));
	set("long","一颗亮晶晶的红石头，好象没什么用的样子，也不知道能不能卖(mai)几个钱。\n");

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/redstone");
	set("rest",0);
	set("desc","好象可以拿到扬州珠宝店换钱。");
	set("credit",20);	
	setup();
}

void init()
{
	add_action("do_mai","mai");
  
} 

int restrict() {
	object me = this_player();

	if (me->query("job_time/丢失的信") < (me->query("quest/chance_times")- me->query("quest/reel")- me->query("quest/creel") +1)*(1000-(me->query("registered")-2)*100)
  || (me->query("job_time/丢失的信") - me->query("quest/used_job")) < (1000-(me->query("registered")-2)*100))
    return notify_fail("你还是多努力一段时间吧。\n");
	
	return  ::restrict();
}

int do_mai(string arg)
{
	object ob,me=this_player();
	    
	if (!arg)
      		return notify_fail("你要卖什么?\n");
      		
	if (arg!="redstone" && arg!="redstone")
      		return notify_fail("你要卖什么?\n");
      	
  if (!(ob=present("zhu laoban", environment(me))))
		return notify_fail("这东西好象不能随便乱卖。\n");    		
  
  if (!living(ob))
		return notify_fail("哦，你得先把他弄醒在说。\n");
		
  if (me->query("job_time/丢失的信") < (me->query("quest/chance_times") - me->query("quest/reel")- me->query("quest/creel") + 1)*(1000-(me->query("registered")-2)*100)
  || (me->query("job_time/丢失的信") - me->query("quest/used_job")) < (1000-(me->query("registered")-2)*100) )
    return notify_fail("这个东西不能卖。\n");
  
  if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}   

	write("朱老板一把抓过赤晶石道：“哇，好东西呀！”\n"NOR);
	write("朱老板道：“这五两"+HIY+"黄金"+NOR+"我就给你存到钱庄里了，你快点去取吧！”\n");	
	write("朱老板把你推到店门外。\n");	
	write(HIW"你稀里糊涂地走到钱庄感觉自己严重被骗了。\n"NOR);
	me->add("balance", 50000);
	me->add("quest/chance",1);
	me->add("quest/chance_times",1);
	me->add("quest/used_job",1000-(me->query("registered")-2)*100);
	me->move("/d/city/qianzhuang");
	degree();
	return 1;
}	 