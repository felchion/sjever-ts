// zijin-dao.c 紫金刀

#include <ansi.h>
#include <weapon.h>
inherit BLADE;

void create()
{
	set_name( HIM"紫金刀"NOR , ({ "zijin dao", "dao","blade" }));
	set_weight(17500);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "把");
		set("long", HIM"这是把紫色的宝刀，只有内功高深的人才能发挥它全部的威力。\n"NOR);
		set("value", 10000);
		set("unique", 1);
		set("rigidity", 3);
		set("weapon_prop/parry", 2);
		set("wield_neili", 500);
		set("wield_maxneili", 900);
		set("wield_str", 22);
		set("treasure",1);
		set("material", "steel");
	}
	init_blade(50);
	setup();
}

int wield()
{
	object ob = environment();
	int ret;

	if( !ob || !living(ob) ) return 0;
	if( (int)ob->query_skill("force") > 150 ) {
		set("weapon_prop/ap" , ob->query_skill("force")/3 );
		if( query("weapon_prop/ap") > 80 )
		set("weapon_prop/ap" , 80 );
	}
	else
	set("weapon_prop/ap" , 30 );
	if(!query("weapon_prop/ap")) return 0;
	else if (ret=::wield()) return ret;
	return 1;
}
