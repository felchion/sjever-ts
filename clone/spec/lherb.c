//flowray 2010.2.13
#include <ansi.h>

inherit SPEC; 

void create()
{
	set_name (HIG"幸运草"NOR, ({ "lherb" , "herb" }));
	set("long","长得有些奇怪的植物，散发出让人昏昏欲睡的幽香。\n你可以使用它("+HIY+"use herb"+ NOR+")。\n");

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/lherb");
	set("desc","可以在打造武器时给人带来好运。");
	set("credit",120);	
	setup();
}

void init()
{
	add_action("do_use","use");
  
}

int restrict() {
	object me = this_player();

	if(  !me->query("imbue_reward")
	&&  me->query("combat_exp")/1000000 < (int)me->query("user_weapon_imbued"))
		return notify_fail("对你来说幸运草暂时没有什么用。\n");
	
	return  ::restrict();
}

int do_use(string arg)
{
	object me=this_player();
	    
	if (!arg)
      		return notify_fail("你使用什么?\n"); 
      		
	if (arg!="herb" && arg!="lherb")
      		return notify_fail("你使用什么?\n");
      	


	write(HIG"你拿起幸运草轻轻一嗅，只见幸运草化做一道绿光消失在你手里。\n"NOR);
	me->add("quest/forg_luck",1);

	degree();
	return 1;
}	 