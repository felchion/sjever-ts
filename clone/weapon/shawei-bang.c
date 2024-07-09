#include <weapon.h>
#include <ansi.h>
inherit STICK;

void create()
{
	set_name(HIR"杀威棒"NOR, ({ "shawei bang","bang" }));
	set_weight(8000);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "根");
		set("long", "一条又粗又长的木制杀威棒，两头红色。\n");
		set("value", 10000000);
		set("unique", 1);
		set("sharpness",1);
		set("wield_str",26);
		set("rigidity",3);
		set("material", "wood");
		set("weapon_prop/ap", 40);
		set("wield_msg", "$N拿出一根两头红色的$n握在手中。\n");
		set("unwield_msg", "$N将手中的$n放下。\n");
	}
	init_stick(80);
	setup();
}
