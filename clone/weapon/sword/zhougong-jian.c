// zhougjian.c 周公剑

#include <weapon.h>
#include <ansi.h>

inherit SWORD;
inherit F_UNIQUE;

void create()
{
	set_name(HIC"周公剑"NOR,({ "zhougong jian", "zhougong", "sword", "jian" }) );
	set_weight(15000);
	if( clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "柄");
		set("value", 1);
		set("rigidity", 4);
		set("sharpness", 5);
		set("unique", 1);
		set("no_get", 1);
		set("material", "steel");
		set("weapon_prop/dodge", -5);
		set("weapon_prop/parry", 5);
		set("treasure",1);
		set("wield_neili", 300);
		set("wield_maxneili", 700);
		set("wield_str", 22);
		set("weapon_prop/ap", 80);
		set("wield_msg", HIG"$N长剑一挺，剑尖上突然生出半尺吞吐不定的青芒。\n"NOR);
		set("long", "这是一把一字慧剑门门主『剑神』卓不凡的佩剑。\n");
		set("unwield_msg", HIY "周公剑幻作一道白光，「唰」地飞入剑鞘。\n" NOR);
	}
	init_sword(120);
	setup();
}
