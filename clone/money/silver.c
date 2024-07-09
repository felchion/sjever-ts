// silver.c

#include <ansi.h>

inherit MONEY;

void create()
{
	set_name(HIW "白银" NOR, ({"silver", "silver_money"}));
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("money_id", "silver");
		set("long", "白花花的银子，人见人爱的银子。\n");
		set("unit", "些");
		set("base_value", 100);
		set("base_unit", "两");
		set("base_weight", 50);
		set("no_get",1);
	}
	set_amount(1);
}

void autoload(string param)
{
	int amt = atoi(param);

	if (amt > environment()->query("combat_exp")/3)
		amt = environment()->query("combat_exp")/3;
	set_amount(amt<1?0:amt);
}
