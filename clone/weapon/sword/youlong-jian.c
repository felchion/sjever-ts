// ITEM: youlong-jian.c
// Date: Look 99/03/25

#include <weapon.h>
#include <ansi.h>
inherit SWORD;

void create()
{
	set_name( YEL "游龙剑" NOR, ({ "youlong jian", "jian", "sword" }));
	set_weight(25000);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "柄");
		set("long", YEL "这是一把看起来非常普通的剑，不过剑上有一些纹路，非常古怪，入手极为沉重。\n" NOR);
		set("value", 1500);
		set("rigidity", 4);
		set("sharpness", 4);
		set("weapon_prop/parry", 4);
		set("material", "steel");
		set("unique", 1);
		set("treasure",1);
		set("wield_neili", 800);
		set("wield_maxneili", 1300);
		set("wield_str", 23);
		set("weapon_prop/ap", 60);
		set("wield_msg", YEL "只听见「飕」地一声，$N的手中已经多了普通的长剑。\n$N略一用劲，陡见剑身的墨纹狂走，犹如狂龙一般缠住剑身，天地为之失色\n" NOR);
		set("unwield_msg", YEL "$N将手中的游龙剑收回腰间，弥漫于天地之间的刚猛狂烈的萧杀之气也随之消失了\n" NOR);
	}
	init_sword(140);
	setup();
}
