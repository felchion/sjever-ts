//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (HIW"天龙八部"NOR, ({ "tianlong","book"}));
	set("long","这是一本查老先生所著的"HIW"天龙八部"NOR"，你可以阅读("HIY"read"NOR")它。\n");

	set("unit","本");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/tianlong");
	//set("rest",1);
	set("desc","据说看了之后会性情大变。");
	set("credit",5);	
	setup();
}

void init()
{
	add_action("do_read","read");
  
}

int do_read(string arg)
{
	object me=this_player();
	    
	if (!arg)
      		return notify_fail("你要阅读什么?\n");
      		
	if (arg!="book")
      		return notify_fail("你要阅读什么?\n");
      	
      	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}   


	me->set("shen",-me->query("shen"));
	
	write(HIY"你细细翻阅这本"HIW"天龙八部"HIY"感受着书中每个角色的喜怒哀乐......\n"NOR);
	write(HIW"你不知不觉地产生了一些情绪波动,你的性情也发生了一些微妙的变化。\n"NOR);
	degree();
	return 1;
}	 