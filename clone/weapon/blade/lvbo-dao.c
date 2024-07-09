// /clone/weapon/lvbo-dao 绿波香露刀

#include <weapon.h>
#include <ansi.h>

inherit BLADE;
inherit F_UNIQUE;

void create()
{
	set_name(GRN"绿波香露刀"NOR, ({ "lvboxianglu dao","lvboxianglu","dao", "blade" }));
	set_weight(20000);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "柄");
		set("rigidity", 4);
		set("sharpness", 4);
		set("material", "steel");
		set("weapon_prop/parry", 4);
		set("long", "这是一把浑身透着暗绿色宝刀，刀身隐隐发出腥臭的气味，却偏偏以香露命名，真是奇怪。\n");
		set("value", 1);
		set("unique", 1);
		set("wield_neili", 800);
		set("wield_maxneili", 1300);
		set("wield_str", 23);
		set("wield_msg", HIG"$N从背后抽出一把宝刀拿在手里，舞了个刀花，但见刀光闪闪，幻出点点寒光。\n"NOR);
		set("unwield_msg", HIG"$N双手一挥，闪电般将刀插回刀鞘内。\n"NOR);
		set("treasure",1);
		set("weapon_prop/ap", 80);
		set("poisoned", "bing_poison");
		set("poison_number", 500);
	}
	init_blade(160);
	setup();
}
