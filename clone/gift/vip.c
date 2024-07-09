
#include <ansi.h>
#include <armor.h>
inherit VIP;
#include <viplvl.h>

string long() {
	int lvl = query("lvl");
	return HIY"这是一张象征尊贵身份的"+Vip_Color(lvl)+vip_type[lvl]+"卡"+NOR+HIY+"。\n"+NOR;
}
string card_name(){	
	if (environment()->query("env/vip_show")) {
		int lvl = query("lvl");
		return Vip_Color(lvl)+vip_type[lvl]+"卡"+NOR;
	}
	return HIG"贵宾卡"NOR;	
}

void create()
{	
	set_name(HIG"贵宾卡"NOR, ({ "VIP Card","card" }) );
	set_weight(100);
	set("long",HIY"这是尊贵身份的象征。\n"NOR);
	set("unit", "张");
	set("value", 3000000);
	set("no_get",1);
	set("no_drop",1);
	set("armor_type", "vip");
	setup();
}

void init()
{
	int lvl ;
	object ob = environment();
	if(objectp(ob))	lvl= ob->query("registered");
	if (lvl>sizeof(vip_type)-1) lvl = sizeof(vip_type)-1;
	if (lvl > 2) {
		set("armor_prop/ap",10*(lvl-2));		
		set("name",(: card_name :));
		set("lvl",lvl);
		set("long",(: long :));
  }
  wear();
}

int unequip() {
	object ob = environment();

	if (ob && userp(ob) 
	&& base_name(previous_object())=="/cmds/std/remove")
		return notify_fail("这个东西卸不下来。\n");
}