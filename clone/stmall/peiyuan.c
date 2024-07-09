#include <ansi.h>

inherit ITEM;

void create()
{
	set_name (HIC"真元丹"NOR, ({ "zhenyuan dan","zhenyuan","dan"}));
	set("long","这是一颗"HIC"真元丹"NOR"，你可以服(fu)下它提高内力修为。\n");

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("desc","提高一千点内力。");
	set("credit",2);
	setup();
}

int restrict() {
		return 1;
}

int do_use(string arg);
void init()
{
	do_use("");  
}

int do_use(string arg)
{
	object me=this_player();
	
	me->add("max_neili", 1000);

	write(HIY"你服下一颗"HIC"真元丹"HIY"顿觉内力大增！\n"NOR);
  
  log_db(me,0,me->query("max_neili"),"真元丹");  
  destruct(this_object());
  
	return 1;
}	 