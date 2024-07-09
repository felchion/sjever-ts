#include <ansi.h>

inherit ITEM;

void create()
{
	set_name (HIC"神奇砥石"NOR, ({"di shi","di","shi"}));
	set("long",HIC"一块神奇的石头，你可以用它磨砺("+HIG+"moli"+HIC+")武器。\n"NOR);

	set("unit","块");
	set_weight(100);
	set("value",3000000);
	set("unique", 1);
	set("treasure",1);

	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);

	setup();
}

void init()
{
	add_action("do_use","moli");
}


int do_use(string arg)
{
	object me=this_player(),wp;
	string wid;

	if(!arg) return notify_fail("你要磨砺什么武器？\n");
	
	if(!objectp(wp = present(arg, me)))
		return notify_fail("你身上没有这样装备。\n");		

	if(me->is_busy()) {return notify_fail("你正忙着呢。\n");}

  if(wp->query("imbued")<4)
  return notify_fail("只有四级武器才能磨砺。\n");
   
	if(query("owner")!=me->query("id"))
	return notify_fail("这不是你的东西。\n");
	
	if(!wp->query("owner") || wp->query("owner")!=me->query("id"))
	return notify_fail("你只能磨砺自己的私有兵器。\n");
	
	
	if(!mapp(wp->query("weapon_mp/weapon_prop_base")) )
	wp->set("weapon_mp/weapon_prop_base",copy(wp->query("weapon_mp/weapon_prop")));

	if(wp->query("imbued")==4 && wp->query("weapon_mp/weapon_prop_base/damage")>400)
	return notify_fail("你只能磨砺自己的私有兵器。\n");
	
	message_vision(HIY"\n$N拿出一块"+HIC+"神奇砥石"+HIY+"用力在"+wp->query("name")+HIY"上磨来磨去，", me);
	message_vision(wp->query("name")+HIY"看起来更加锋利了一些。\n"NOR, me);

	
	wid=wp->query("save_id");
	wp->add("weapon_mp/weapon_prop_base/damage",1);
	wp->add("weapon_mp/weapon_prop_base/up_dmg",1);

	wp->save_weapon(me);
 	destruct(wp);
	wp=WORKER_D->load_user_object(me ,me->query("worker/"+wid));		
	destruct(this_object());
	return 1;
}