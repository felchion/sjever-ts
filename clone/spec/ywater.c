//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (HIG"青春之泉"NOR, ({"ywater","qingchun quan"}));
	set("long",HIC"这是一个瓶来自一个未知世界的"HIG"青春之泉"HIC"，据说喝(he)它之后能重返青春。\n");

	set("unit","个");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1); 
	
	set("degree",1);
	set("flag","spec/time");
	set("rest",0);
	set("desc","据说喝了之后可以年青一岁。");
	set("credit",15);	
	setup();
}


int restrict() {
	object me = this_player();

	if (me->query("age") - me->query("age_reyoung") < 18)
		return notify_fail("你已经很年轻了。\n");
	
	return  ::restrict();
}

void init()
{
	add_action("do_use","he");
  
}

int do_use(string arg)
{
	object me=this_player();

	if (!arg)
		return notify_fail("你要使用什么?\n");

	if (arg!="water" && arg!="quan")
		return notify_fail("你要使用什么?\n");

	if (me->query("age") - me->query("age_reyoung") < 18)
		return notify_fail("你已经很年轻了。\n");

	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}

  me->add("age_reyoung",1);
		
	write(HIW"你喝下一瓶"HIG"青春之泉"HIW"，只觉一股暖流在全身流动，你突然感觉到自己又充满了活力。\n"NOR);
	degree();
	return 1;
}