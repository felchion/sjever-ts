#include <weapon.h>
#include <ansi.h>

inherit BLADE;
inherit F_UNIQUE;
void create()
{
	set_name(HIY"斩妖金铙"NOR,({ "zhanyao nao", "nao","zhanyao" }) );
	set_weight(15000);
	if( clonep() )
	set_default_object(__FILE__);
	else {
		set("treasure", 1);
		set("unit", "副");
		set("value", 5000);
		set("unique", 1);
		set("rigidity", 3);
		set("sharpness", 3);
		set("weapon_prop/parry", 2);
		set("material", "gold");
		set("wield_neili", 500);
		set("wield_maxneili", 900);
		set("wield_str", 22);
		set("weapon_prop/ap", 40);
		set("wield_msg",HIY"只听见「唰」地一声，斩妖金铙飞出$N手中，闪出道道金光，威声震天！\n"NOR);
		set("long", HIY"\n这是用纯金打造的一副金铙的左半副，直径二尺有三，上面刻满斩妖法诀。\n"+
		"如果找到右半副，应该可以将两副金铙合起来。\n"NOR);
		set("unwield_msg", HIY "斩妖金铙向半空飞出，又飞回了$N的手中。\n"NOR);
	}
	init_blade(70);
	setup();
}

