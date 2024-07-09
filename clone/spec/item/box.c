
#include <ansi.h>
#include <armor.h>

inherit ITEM;

void create()
{	
	set_name(HIW"水晶礼盒"NOR, ({ "box","li he" }) );
	set("long", "一个用水晶做成的小盒子。\n");
	set("unit","个");
	set("no_drop",1);
	set("no_give",1);
	
	setup();
}

void init()
{
	if((this_player() == environment()) && (this_player()->query_temp("token/maker")))
	{
		set("long", "一个装着礼物的"+HIW"水晶礼盒"NOR+",你可以把它赠送(present)给你的亲友。\n");
	}


	add_action("do_apply", "apply");
	add_action("do_present", "present");

}

int do_present(string arg)
{
	object ob,me = this_player();
	
	if (!me->query_temp("token/maker"))
		return notify_fail("这个东西不能转送。\n");
		
	if (!arg)
		return notify_fail("你要把东西赠送给谁啊？\n");
    
	if (!(ob = present(arg, environment(me))))
		return notify_fail("这里没有这个人。\n");
	
	if (!userp(ob))
		return notify_fail("你还是不要自做多情了。\n");

	if (!living(ob))
		return notify_fail("你得先把人家弄醒呀。\n");
	
	if (ob->query_temp("token/maker") && ob != me)
		return notify_fail(ob->query("name")+"暂时不能接受你的礼物，你等等再送吧。\n");
		
	if (ob->query_temp("token/apply"))
		return notify_fail(ob->query("name")+"已经收到一个礼物了，你等等再送吧。\n");
		
//	if (ob->query_temp("token/maker"))
//		return notify_fail(ob->query("name")+"现在不能接受礼物。\n");
		
//	if (ob->query("token/tranfered"))
//		return notify_fail(ob->query("name")+"不能接受你的礼物。\n");
		
	if (mapp(ob->query("token/"+query("token/id"))))
		return notify_fail(ob->query("name")+"已经保存了一件同类的纪念品，不能再接受你的馈赠了。\n");

		
	set("long", "这是"+this_player()->query("name")+"赠送给你的"+query("name")+"你可以接受(apply)它。\n");

	me->delete_temp("token/maker");
	if (ob == me) {
			me->set("token/"+query("token/id")+"/desc",query("token/desc"));
			if (query("token/parry"))
				me->set("token/"+query("token/id")+"/parry",query("token/parry"));
			if (query("token/dodge"))
				me->set("token/"+query("token/id")+"/dodge",query("token/dodge"));
			if (query("token/ap"))
				me->set("token/"+query("token/id")+"/ap",query("token/ap"));	
			ob=new("clone/spec/item/"+query("token/id")+".c");
			ob->move(me);
			tell_object(me, "你打开水晶盒子取出里面的"+ob->query("unit")+ob->query("name")+"给自己戴上了，随手把水晶盒子丢在一边。\n");	
			destruct(this_object());
			return 1;
	}
	ob->set_temp("token/apply",1);
	this_object()->move(ob);
	tell_object(me, "你很小心地从怀里拿出一个"+query("name")+"交给"+ob->query("name")+"。\n");
	tell_object(ob,me->query("name")+"很小心地从怀里拿出一个"+query("name")+"交给你。\n");
	log_file("static/spec/item.c",
		strip(sprintf("%18s赠送首饰给%18s。\n",
		me->query("name")+"("+
		me->query("id")+")",
		ob->query("name")+"("+
		ob->query("id")+")")
		));
	return 1;
}

int do_apply()
{
	object ob,me = this_player();

	if (!me->query_temp("token/apply"))
		return notify_fail("已经装好了就不要再看了吧。\n");
		
	if (mapp(me->query("token/"+query("token/id"))))
		return notify_fail("你已经保存了一件纪念品，不能再接受别人的馈赠了。\n");
			
	me->delete_temp("token");
	me->set("token/"+query("token/id")+"/desc",query("token/desc"));
	if (query("token/parry"))
		me->set("token/"+query("token/id")+"/parry",query("token/parry"));
	if (query("token/dodge"))
		me->set("token/"+query("token/id")+"/dodge",query("token/dodge"));
	if (query("token/ap"))
		me->set("token/"+query("token/id")+"/ap",query("token/ap"));	
	ob=new("clone/spec/item/"+query("token/id")+".c");
	ob->move(me);
	tell_object(me, "你打开水晶盒子惊奇地发现里面有一"+ob->query("unit")+ob->query("name")+"，你开心的戴上了"+ob->query("name")+"，随手把水晶盒子丢在一边。\n");	
	destruct(this_object());
	return 1;
}
