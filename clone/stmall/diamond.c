#include <ansi.h>

inherit ITEM; 

void create()
{
	set_name (HIR"血钻"NOR, ({ "diamond" , "zuan" }));
	set("long","一颗鲜红色亮晶晶的小石头,好象没什么用的样子，也不知道能不能卖(mai)几个钱。\n");

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	

	set("desc","可换取一千锭黄金。");
	set("credit",3);	
	setup();
}


int restrict() {
		object me = this_player();
		if (me->query("balance")+ 1000000 > F_BANKER->query_balance_limit(me))
		  return notify_fail("您的存款限额太低，无法兑换钻石。\n");
		
		return 1;
}

 
int do_mai()
{
	object me=this_player();

	write("朱老板不知道从哪里跑出来，一把抓过"HIR"血钻"NOR"道：“哇，好东西呀！”\n"NOR);
	write("朱老板道：“一千锭黄金我就给你存到钱庄里了，你快点去取吧！”\n");
	write("朱老板一溜烟又跑不见了。\n");	
	write(HIW"你一脸懵逼，感觉自己严重被骗了。\n"NOR);
	me->add("balance", 10000000);

	log_db(me,0,me->query("balance"),0,"血钻");

	destruct(this_object());
	return 1;
}

void init()
{
	do_mai(); 
}