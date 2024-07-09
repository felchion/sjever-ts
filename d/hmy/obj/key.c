// key.c 监狱钥匙
// Created by Numa 19991023

#include <ansi.h>
inherit ITEM;
#include "hmy_def.c"
#define JAIL_GATE "/d/hmy/jail_gate"

void create()
{
	set_name(HIB"黑钥匙"NOR,({"hei yaoshi", "yaoshi", "key"}));
	set_weight(1000);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long","这是一把青铜制成的黑色钥匙。\n");
		set("material", "steel");
		set("unit", "把");
		set("value", 2000);
		set("unique", 1);
		set("treasure", 1);
		set("no_give",1);
		set("no_get",1);
	}
	setup();
}

/*
void init()
{
	object ob = this_player();
	object me = this_object();
	
	if (present("yao shi",ob)
	&&  me->query_temp("owner") == ob->query("id"))
		add_action("do_unlock",({"unlock", "kai"}));
}

int do_unlock(string arg)
{
	object ob = this_player();
//	object me = this_object();
	object room,here,obj,another,npc1;

	if (!arg)
		return notify_fail("你要打开什么？\n");
	if (arg != "men" && arg != "门")
		return notify_fail("你要打开什么？\n");

	here = environment(ob);
	room = load_object(__HMY__"jail");
	if (base_name(here) != JAIL_GATE)
		return notify_fail("这里好象没有门呀？\n");
	another = present(ob->query_temp("hostage/partner"),here);
	if (!another)
		return notify_fail("你的同伴还没有来，等一下他吧。\n");
	if (!(ob->query_temp("hostage/ready_3")
	  || another->query_temp("hostage/ready_3")))
	  	return notify_fail("你们的任务好象还没有完成呢！\n");
	message_vision(HIG"\n$N从容的拿出钥匙，只听「啪」的一声，门锁打开了。\n"NOR, ob);
	
	obj = present(ob->query_temp("hostage/hostage_i"),room);
	if (!obj)
		return notify_fail("咦？人质呢？\n");
	if (userp(obj)) {
		tell_object(obj,HIB"\n你正在纳闷如何出去时，大门「哐铛」一声打开了。\n"NOR);
		obj->move(here);
		tell_room(here,obj->name() + "从"HIB"监狱"NOR"里走了出来。\n",({obj}));
		tell_object(obj,"你急忙走了出去，你发现原来是"+ ob->name() +"和"+ another->name() +"一起救你出来的。\n");
		tell_object(obj,"你被"+ another->name() +"送了出去。\n");
		obj->move(obj->query_temp("hostage/where"));
		obj->delete_temp("hostage/where");
	}
	else {
		obj->move(here);
		tell_room(here,obj->name() + "从"HIB"监狱"NOR"里走了出来。\n",({obj}));
		message_vision("$n一出监狱，一个箭步就跑的不见了影儿。",obj);
		obj->move(obj->query_temp("hostage/where"));
		obj->delete_temp("hostage/where");
		obj->reincarnate();
	}
	tell_room(here,HIR"\n就在这时，日月神教总管杨莲亭冲了过来，看见了你们之后，扭头就跑！\n"NOR);
	message_vision(HIY"\n$N回头说道：“我去追他，咱们客栈回合。”说完就匆匆忙忙的去追赶逃跑的杨莲亭。\n"NOR,ob);
	ob->delete_temp("hostage/ready_3");
	ob->set_temp("hostage/job_3",1);
	ob->apply_condition("hostage3",60 + random(20));
	npc1 = new(__HMY__"npc/yang");
	npc1->set_temp("target2",ob->query("id"));
	npc1->move(__HMY__"xiaohuayuan");
	ob->move(__HMY__"xiaohuayuan");
	message_vision(HIY"\n$N对着$n大叫道：“"+ RANK_D->query_rude(npc1) +"，哪里跑，站住！”\n"NOR, ob, npc1);
	message_vision(HIR"$n急忙收步，对着$N狞笑道：“既然你不肯放过我，也休怪我无情！”\n"NOR, ob, npc1);
	message_vision(HIY"\n$N立刻准备动身离开这里，刚走了两步，远远看见来了几个人！\n"NOR, another);
	message_vision(HIR"原来是魔教三长老赶到了。\n"NOR, another);
	another->delete_temp("hostage/ready_3",);
	another->set_temp("hostage/job_3", 2);
	
    	another->apply_condition("hostage3",60 + random(20));
	npc1 = new(__HMY__"npc/killer5");
	npc1->set_temp("target",another->query("id"));
	npc1->move(here);
	npc1 = new(__HMY__"npc/killer6");
	npc1->set_temp("target",another->query("id"));
	npc1->move(here);
	npc1 = new(__HMY__"npc/killer7");
	npc1->set_temp("target",another->query("id"));
	npc1->move(here);
	return 1;
}
*/