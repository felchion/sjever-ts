//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (YEL"龙阳散"NOR, ({ "ly san","san"}));
	set("long","这是一包"HIY"龙阳散"NOR"，据说吃(eat)了之后可以重塑男儿之身。\n");

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/lysan");
	set("rest",0);
	set("desc","重塑男儿之身。");
	set("credit",20);	
	setup();
}

void init()
{
	add_action("do_pray","eat");
  
}

int restrict()
{
	object me=this_player();
	if (me->query("gender")!="无性" )
		return 0;
	return ::restrict();
}

int do_pray(string arg)
{
	object me=this_player();
	string str;

	if (!arg)
	return notify_fail("你要吃什么?\n");

	if (arg!="san")
	return notify_fail("你要吃什么?\n");

	if (!restrict()) 
		{return notify_fail("你现在不能使用"+this_object()->query("name")+"。\n");}
	
	me->set("gender","男性");
	if (me->query("class") == "eunuch")
	me->delete("class");
	if (me->query_skill("pixie-jian",1)){
		tell_object(me,HIR"你心中回忆起当年练辟邪剑法的点点滴滴，终于决定要放弃。\n"NOR);
		me->delete_skill("pixie-jian");
	}
	str = HIW"你吃下一颗"+query("name")+HIW"，顿时觉得浑身为之一振，";
	str += "恢复了真男之身。\n";

	write(str+NOR);

	degree();
	return 1;
}