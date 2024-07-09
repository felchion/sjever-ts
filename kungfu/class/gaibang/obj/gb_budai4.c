
#include <ansi.h>
#include <armor.h>

inherit WAIST;

void create()
{
	set_name(YEL"四个布袋"NOR, ({ "bu dai", "budai", "dai", "bag" }));
	set_weight(400);
	set_max_encumbrance(400);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "四只用粗麻布织成的袋子，身上所带布袋的多少，是丐帮弟子江湖地位的标志。\n");
		set("material", "cloth");
		set("unit", "个");
		set("value", 0);
		set("remove_msg", "$N将身上的布袋除了下来。\n");
		set("wear_msg", "$N将布袋小心地别在身后。\n");
		set("armor_prop/armor", 4);
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
