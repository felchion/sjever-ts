//flowray 2008.1.12
#include <ansi.h>

inherit SPEC; 

void create()
{
	set_name (HIW"钻石"NOR, ({ "diamond" , "zuan" }));
	set("long","一颗亮晶晶的小石头,好象没什么用的样子，也不知道能不能卖(mai)几个钱。\n");

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/diamond");
	set("rest",1);
	set("desc","好象可以拿到扬州珠宝店换钱。");
	set("credit",15);	
	setup();
}

void init()
{
	add_action("do_mai","mai");
  
}

int restrict() {
		object me = this_player();
		if (me->query("balance")+ 5000000 > F_BANKER->query_balance_limit(me))
		  return notify_fail("您的存款限额太低，无法兑换钻石。\n");
		else
		  return  ::restrict();
}

int do_mai(string arg)
{
	object ob,me=this_player();
	    
	if (!arg)
      		return notify_fail("你要卖什么?\n");
      		
	if (arg!="diamond" && arg!="zuan")
      		return notify_fail("你要卖什么?\n");
      	
  if (!(ob=present("zhu laoban", environment(me))))
		return notify_fail("这东西好象不能随便乱卖。\n");    		
  
  if (!living(ob))
		return notify_fail("哦，你得先把他弄醒在说。\n");
		
  if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}   

	write("朱老板一把抓过钻石道：“哇，好东西呀！”\n"NOR);
	write("朱老板道：“这五百两黄金我就给你存到钱庄里了，你快点去取吧！”\n");	
	write("朱老板把你推到店门外。\n");	
	write("你稀里糊涂地走到钱庄感觉好象被骗了。\n");
	me->add("balance", 5000000);
	me->move("/d/city/qianzhuang");
	degree();
	return 1;
}	 