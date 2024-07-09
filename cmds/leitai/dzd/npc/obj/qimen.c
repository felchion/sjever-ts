// /d/dzd/obj/qimen.c

#include <ansi.h>
inherit ITEM;

void create()
{
	set_name(HIY"奇门八卦图谱"NOR,({"qimenbagua tupu","tupu"}));
	set_weight(10);
	if(clonep())
		set_default_object(__FILE__);
	else {
		set("unit","本");
		set("long","这是一本记载着九宫八卦奥秘的图谱，据说是凝聚了黄老邪毕生的心血。
随便学学，应该能自己布阵(buzhen)、撤阵(chezhen)了吧。\n");
		set("value",100);
		set("material","paper");
	}
	setup();
}

void init()
{
	add_action("do_buzhen","buzhen");
	add_action("do_chezhen","chezhen");
}

int do_buzhen()
{
	object ob;	
	object me = this_player();
	
	if( environment(me)->query("no_fight"))
	 	return notify_fail("这里不准战斗。\n");
	if(me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");
	if(me->query_temp("dzd_quest/buzhen"))
		return notify_fail("你已经布阵了。\n");
	if(present("qimenbagua zhenxing",environment(me)))
		return notify_fail("已经有人在这里布阵了。\n");
	message_vision(HIY"$N东搬一块，西移一块，不一会就按图谱所示摆了一个简易的阵行！\n"NOR,me);
	me->set_temp("dzd_quest/buzhen",1);
	ob = new(__DIR__"zhenxing");
	ob->set_temp("zhen_master",this_object() );
	ob->move(environment(me));
	return 1;
}

int do_chezhen(string arg)
{
	object ob;
	object me = this_player();
	if(me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");
	if(!arg) arg = "qimenbagua zhenxing";
	ob = present(arg,environment(me));
	if(!ob)
		return notify_fail("你没有在这里布阵！\n");
	if(ob->query_temp("zhen_master") != this_object()  )
		return notify_fail("这不是你布的阵！\n");
	message_vision(HIY"$N三下五除二就把刚刚布好的简陋阵形撤掉了！\n"NOR,me);
	me->delete_temp("dzd_quest/buzhen");
	destruct(ob);
	return 1;
}