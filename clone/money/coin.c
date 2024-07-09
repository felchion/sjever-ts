// coin.c
#include <ansi.h>
#include <weapon.h>

inherit MONEY;
inherit THROWING;

void create()
{
	set_name(YEL "铜钱" NOR, ({"coin", "coins", "coin_money" }));
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("money_id", "coin");
		set("long", "这是流通中单位最小的货币，约要一百文铜钱才值得一两白银。\n");
		set("unit", "些");
		set("base_value", 1);
		set("no_get",1);
		set("base_unit", "文");
		set("base_weight", 5);
	}
	set_amount(1);
	init_throwing(50);
}

void autoload(string param)
{
	int amt = atoi(param);

	if (amt/30 > environment()->query("combat_exp"))
		amt = environment()->query("combat_exp")*30;
	set_amount(amt<1?0:amt);
}
