//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (HIB"轮回之书"NOR, ({ "rbook","book"}));
	set("long","这是本蕴有轮回之力的"HIB"轮回之书"NOR"，你可以阅读("HIY"read"NOR")它。\n");

	set("unit","本");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/tianlong");
	//set("rest",1);
	set("desc","减少十次死亡记录。");
	set("credit",10);	
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


	me->add("death_count",-10);
	if (me->query("death_count")<0) 
		me->set("death_count",0);
	
	write(HIY"你打开这本"HIB"轮回之书"HIY"，一股奇异的力量从书弥散开来......\n"NOR);
  me->unconcious(); 
	degree();
	return 1;
}	 