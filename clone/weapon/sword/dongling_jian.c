#include <weapon.h>
#include <ansi.h>

inherit SWORD;
inherit F_UNIQUE;

void create()
{
	set_name(CYN"东灵铁剑"NOR, ({ "dongling tiejian", "tiejian", "jian", "sword" }));
	set_weight(16000);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "柄");
		set("long", "一柄黑黝黝的铁铸短剑，剑身上刻着一行篆文【见此铁剑，如见东灵】。\n");
		set("value", 1500);
		set("unique", 1);
		set("sharpness", 3);
		set("rigidity", 3);
		set("treasure", 1);
		set("weapon_prop/parry", 2);
		set("wield_neili", 500);
		set("wield_maxneili", 900);
		set("wield_str", 22);
		set("material", "steel");
		set("weapon_prop/ap", 40);
		set("wield_msg", "$N「唰」的一声抽出一柄$n握在手中。\n");
		set("unwield_msg", "$N将手中的$n插回剑鞘。\n");
	}
	init_sword(90);
	setup();
}
