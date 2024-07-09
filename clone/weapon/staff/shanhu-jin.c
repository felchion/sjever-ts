//shanyu-jin 珊瑚金
#include <weapon.h>
#include <ansi.h>
inherit STAFF;

void create()
{
	set_name(BLU"珊瑚金"NOR, ({ "shanhu jin", "shanhu","jin" }) );
	set_weight(32000);
	if( clonep() )
	set_default_object(__FILE__);
	else {
		set("unit", "根");
		set("long", "这是一根珊瑚做成的拐杖，是灵蛇岛旁海底的特产，由数种金属混和，\n"
		"在海底中厉千万年而成珊瑚，削铁如切豆腐，打石如敲棉花，不论多锋\n"
		"利的利刃，遇之立折。\n"
		);
		set("rigidity", 4);
		set("weapon_prop/parry", 4);
		set("wield_neili", 900);
		set("wield_maxneili", 1400);
		set("wield_str", 24);
		set("unique", 1);
		set("treature", 1);
		set("value", 1000);
		set("material", "steel");
		set("weapon_prop/ap", 240);
		set("wield_msg", "$N拿出一根$n，握在手中。\n");
		set("unwield_msg", "$N放下手中的$n。\n");
	}
	init_staff(140);
	setup();
}
