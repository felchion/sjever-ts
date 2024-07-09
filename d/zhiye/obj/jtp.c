

#include <ansi.h>

inherit MONEY;

void create()
{
	set_name(HIW "精铁魄" NOR, ({"jingtie po", "tiepo","po"}));
	if( clonep() )
	set_default_object(__FILE__);
	else {
		set("long", HIW"闪烁着奇异光泽的白色金属！\n"NOR);
		set("unit", "些");
		set("base_value", 1);
		set("base_unit", "块");
		set("base_weight", 1);
		set("no_keep", 1);
		set("forg_id", "jingtie po");
	}
	set_amount(1);
}


void autoload(string param)
{
	int amt = atoi(param);

	set_amount(amt);

}

void init()
{
	object me = this_player();
	if (me && present(this_object(), me))	{
		add_action("do_put","put");
	}
}

int do_put(string arg){
	object me = this_player();
	if (!arg) return 0;
	if (arg = "jingtie po to bag") {
		me->add("collection/l1/jingtie po",query_amount());
		write("你把"HIW"精铁魄"NOR"收进了背囊。\n");
		destruct(this_object());
		return 1;
  }
  return 0;
}