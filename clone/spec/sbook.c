//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
  set_name (HIC"素质之书"NOR, ({ "sbook"}));
  set("long","这是一本"HIC"素质之书"NOR"，你可以阅读(read)有关臂力(str)根骨(con)身法(dex)悟性(int)方面的知识。\n");

  set("unit","本");
  set_weight(100);
  set("value",3000000);
    
  set("no_get",1);
  set("no_drop",1);
  set("no_get_from",1);
  
  set("degree",1);
  set("flag","spec/sbook");
  set("desc","暂时性提高五点天赋。");
  set("credit",5);  
  setup();
}

void init()
{
  add_action("do_read","read");
  
}

int restrict() {
	object me = this_player();

	if (me->query_temp("sbook/count")>7)
		return notify_fail("你己经很厉害了。\n");
	
	return  ::restrict();
}


int do_read(string arg)
{
  object me=this_player();
  
  if (!arg)
    return notify_fail("你要阅读什么知识?\n");
  
  if (arg!="dex" && arg!="int" && arg!="con" && arg!="str")
    return notify_fail("你要阅读什么知识?\n");
  
  if (me->query_temp("sbook/count")>7)
    return notify_fail("你看了看这本书，发现已经没什么好学的了!\n");

  if (me->query_temp("sbook/"+arg)>1)
    return notify_fail("你已经无法通过素质之书提高这种属性!\n");
      
  if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}  

  me->add_temp("apply/"+arg,5);
  me->add_temp("sbook/"+arg,1);
  me->add_temp("sbook/count",1);
  
  write(HIY"你读完"HIC"素质之书"HIY"发现自己好象变得厉害了一点。\n"NOR);
  degree();
  return 1;
}