// bamboo_stick.c 玉竹棒
// by shang 97/6
// modified by explorer, 9/11/97

#include <ansi.h>
#include <weapon.h>

inherit STICK;


void create()
{

	set_name(HIG "玉竹棒" NOR, ({"yuzhu bang", "bang", "stick"}));
	set_weight(10000);

	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "根");
		set("long",HIG + "一根看起来温润晶莹，通体碧绿的竹棒，泛着淡淡的青光 。\n" NOR);
		set("value", 1000);
		set("material", "bamboo");
		set("treasure", 1);
		set("unique", 1);
		set("weapon_prop/parry", 4);
		set("rigidity", 10);
		set("wield_neili", 900);
		set("wield_maxneili", 1400);
		set("wield_str", 24);
		set("weapon_prop/ap", 100);
		set("wield_msg", "$N抽出一根青光闪闪的玉竹杖抓在手中。\n");
		set("unwield_msg", "$N把手中的玉竹杖插回腰中。\n");

	}
	init_stick(160);
	setup();
}     