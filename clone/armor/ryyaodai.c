// ryyaodai.c 软玉腰带
// creat by Lklv 2001.9.28

#include <ansi.h>
#include <armor.h>

inherit WAIST;

void create()
{
	set_name( HIC"软玉腰带"NOR, ({ "yao dai", "yaodai", "dai" }) );
	set_weight(400);
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("unit", "条");
                set("long", "这是一件上好的蓝田软玉做成的腰带。\n");
		set("value", 5000);
		set("material", "waist");
		set("wear_msg", "$N看了看周围没人，低头将一条"HIC"软玉腰带"NOR"系在腰上。\n");
		set("remove_msg", "$N潇洒地将"HIC"软玉腰带"NOR"自腰间解下，放在手里。\n");
		set("armor_prop/armor", 5);
	}
	setup();
}
