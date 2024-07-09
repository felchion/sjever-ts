// /d/dzd/obj/pimao.c

#include <armor.h>
inherit HEAD;

void create()
{
	set_name("皮帽",({"pi mao","mao"}));
	set_weight(50);
	if(clonep())
		set_default_object(__FILE__);
	else {
		set("long","这是一顶当年史家五兄弟驯兽时留下的皮帽。据说有奇妙的用处！\n");
		set("value",10000);
		set("unit","顶");
              	set("armor_prop/armor", 60);
              	set("no_animal",1);
              	set("material", "cloth");
		set("wear_msg","$N拿起一顶皮帽，戴在了头上。\n");
	}
	setup();
}
