//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (HIW"幽昙奇花"NOR, ({ "youtan","flower"}));
	set("long","这是一朵生长在千年冰川里的"HIW"幽昙奇花"NOR"，据说对它能让人青春永驻?
你可以服用（"HIY"fu"NOR"）它来提高容貌n");

	set("unit","朵");
	set_weight(100);
	set("value",3000000);
	
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/youtan");
	set("desc","增加一点容貌。");
	set("credit",100);
	
	setup();
}

void init()
{
	add_action("do_eat","fu");
  
}

int restrict() {
	object me = this_player();

	if (me->query("age")<16)
		return notify_fail("你年龄太小，无法承受药性。\n");
		
	if (me->query("per")>29)
		return notify_fail("你己经够漂亮了。\n");
	
	return  ::restrict();
}

int do_eat(string arg)
{
	object me=this_player();
	    
	if (!arg)
      		return notify_fail("你要服用什么?\n");
      		
	if (arg!="flower" && arg!="youtan")
      		return notify_fail("你要服用什么?\n");
      	
      	if (me->query("per")>29)
      		return notify_fail("你已经很漂亮了!\n");
      		
      	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}   

	me->add("per",1);
	write(HIW"你服下一朵幽昙奇花觉得一股清凉地感觉流遍了你的全身。\n"NOR);
	if (me->query("per")>29)
		 write(HIY"你觉得你自己已经美得不能再美了。\n"NOR);
	degree();
	return 1;
}	 