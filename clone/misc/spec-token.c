
#include <ansi.h>
#include <armor.h>

inherit HANDS;

void create()
{	
	if((this_player() == environment()) && (this_player()->query_temp("token/maker")))
	{
		set_name(this_player()->query_temp("token/name"), ({ this_player()->query_temp("token/id") }) );
		set("long", "这是珠宝店里的秘藏品,你可以把它赠送(present)给你的亲友。\n");}
		set("unit",this_player()->query_temp("token/unit"));
		if (this_player()->query_temp("token/parry"))
			set("armor_prop/parry",query_temp("token/parry"));
		if (this_player()->query_temp("token/dodge"))
			set("armor_prop/dodge",query_temp("token/parry"));
		if (this_player()->query_temp("token/armor"))
			set("armor_prop/armor",query_temp("token/parry"));
	else
	{			
		set_name(this_player()->query("token/name"), ({ this_player()->query("token/id") }) );
		set("long", this_player()->query("token/desc")+"\n");
		set("unit",this_player()->query("token/unit"));
		if (this_player()->query("token/parry")) 
			set("armor_prop/parry",this_player()->query("token/parry"));
		if (this_player()->query("token/dodge"))
			set("armor_prop/dodge",this_player()->query("token/dodge"));
		if (this_player()->query("token/armor"))
			set("armor_prop/armor",this_player()->query("token/armor"));
	}
	
	setup();
}

int query_autoload()
{

	return 1;
}

void init()
{
	if((this_player() == environment()) && (this_player()->query_temp("token/maker")))
	{
		add_action("do_present", "present");

	}

}

int do_present(string arg)
{
	object ob,me = this_player();
	
	
	if (!arg)
		return notify_fail("你要把东西赠送给谁啊？\n");
    
	if (!(ob = present(arg, environment(me))))
		return notify_fail("这里没有这个人。\n");
	
	if (!userp(ob) )
		return notify_fail("你还是不要自做多情了。\n");

	if (!living(ob))
		return notify_fail("你得先把人家弄醒呀。\n");
			
	add_action("do_apply", "apply");
	set("long", "这是"+this_player()->query("name")+"赠送给你的"+query("name")+"你可以接受(apply)它。\n");
	this_player()->delete_temp("token");
	this_object()->move(ob);
	tell_object(me, "你很小心地从怀里拿出"+query("name")+"递给"+ob->query("name")+"。\n");
	tell_object(ob,me->query("name")+"很小心地从怀里拿出"+query("name")+"递给你。\n");
}

