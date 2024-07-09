
#include <ansi.h>
#include <armor.h>

inherit WAIST;

void create()
{
	set_name("一个布袋", ({ "bu dai", "budai", "dai", "bag" }));
	set_weight(100);
	set_max_encumbrance(100);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "一只用粗麻布织成的袋子，身上所带布袋的多少，是丐帮弟子江湖地位的标志。\n");
		set("material", "cloth");
		set("unit", "个");
		set("value", 0);
		set("remove_msg", "$N将布袋除了下来。\n");
		set("wear_msg", "$N将布袋别在身后。\n");
		set("armor_prop/armor", 1);
		set("no_get","身为丐帮弟子，袋在人在，袋亡人亡！\n");
		set("no_drop", "身为丐帮弟子，袋在人在，袋亡人亡！\n");
		set("no_give", "身为丐帮弟子，袋在人在，袋亡人亡！\n");
        }
	set("armor_type", "gaibang");
        setup();
}

int unequip()
{
	object ob = environment();
	if (ob)
		return notify_fail("布袋是丐帮弟子身份的象征，你不能把它除下！\n");
	return ::unequip();
}
