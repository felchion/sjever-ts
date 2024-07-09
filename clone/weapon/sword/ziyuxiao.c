// ziyuxiao.c 
#include <weapon.h>
#include <ansi.h>
inherit SWORD;

void create()
{
	set_name(MAG"紫玉箫"NOR,({"ziyu xiao","xiao"}));
	set_weight(5000);
	if (clonep())
	set_default_object(__FILE__);
	else{
		set("unit","支");
		set("value",6000);
		set("material","gem");
		set("unique", 3);
		set("treasure", 1);
		set("rigidity", 3);
		set("wield_neili", 500);
		set("wield_maxneili", 900);
		set("weapon_prop/parry", 2);
		set("weapon_prop/ap", 40);
		set("long","这是一支晶莹剔透的用罕见的"MAG"紫玉"NOR"做的箫，看起来十分珍贵。\n");
		set("wield_msg","$N伸手轻轻一招，一支$n已出现在$N手中。\n");
		set("unwield_msg","$N一声清啸，将$n插回腰间。\n");
	}
	init_sword(80);
	setup();
}
int wield()
{
	object me = environment();
	if (living(me) && me->query_skill("yuxiao-jian", 1) < 100){
		message_vision("$N拿起"+name()+"，但是不会使用。\n", me);
		return 0;
	}
	if(!query("weapon_prop/damage")) return 0;
	return ::wield();
}

