//flowray 2005.6.16
#include <ansi.h>

inherit SPEC;

void create()
{
	set_name (HIW"水晶盒"NOR, ({ "crystal box","box"}));
	set("long","一个晶莹剔透盒子，可以用来包装("HIY"pack"NOR")首饰。\n");

	set("unit","个");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/box");
	set("desc","可以用来包装首饰。");
	set("credit",10);
	setup();
}

void init()
{
	add_action("do_pack","pack");   
}

int do_pack(string str)
{
	
	object ob1,ob2,me = this_player();
	
	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}
	
	if (!str)
		return notify_fail("你包装什么东西？\n");
    
	if (!(ob1=present(str, me)))
		return notify_fail("你没有这样首饰！\n");

  if (!me->query("token/"+str))
 		return notify_fail("你没有这样首饰！\n");
 			
	if (!function_exists("token_reload", ob1))
		return notify_fail("你不能把这样东西赠送给别人。\n");
	
	if (me->query_temp("token/maker"))
		return notify_fail("你要先把包装好的送出去。\n");
	
	me->set_temp("token/maker",1);
	
	ob2=new("/clone/spec/item/box.c");
	ob2->set("token/id",str);
	
	if (me->query("token/"+str+"/parry"))
		ob2->set("token/parry",me->query("/token/"+str+"/parry")-1-random(3));
	if (me->query("token/"+str+"/dodge"))
		ob2->set("token/dodge",me->query("/token/"+str+"/dodge")-1-random(3));
	if (me->query("token/"+str+"/ap"))
		ob2->set("token/ap",me->query("/token/"+str+"/ap")-1-random(3));	
	ob2->set("token/desc",me->query("/token/"+str+"/desc"));
	write("你褪下一"+ob1->query("unit")+ob1->query("name")+"装进"+HIW+"水晶盒子"+NOR+"里。\n");
	ob2->move(me);
	me->add_temp("apply/eq_armor",-(ob1->query("armor_prop/armor")));
	me->add_temp("apply/ap",-(ob1->query("armor_prop/ap")));
	destruct(ob1);
	me->delete("token/"+str);
	me->add("token/tranfered",1);
	degree();
	return 1;
}	 

