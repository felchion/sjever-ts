// OBJ : /d/xiangyang/npc/obj/qipao.c 缎子旗袍
// By Lklv 2001.9.26

#include <ansi.h>
#include <armor.h>
inherit CLOTH;

void create()
{
	set_name(HIM"旗袍"NOR, ({"qi pao", "qipao", "pao"}) );
	set_weight(3500);
	if(clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "件");
		set("long", HIC"一件紫色旗袍，领口绣了些碎花。\n"NOR);
		set("value", 5000);
		set("material", "cloth");
		set("armor_prop/armor", 2);
		set("female_only", 1);
	}
	setup();
}
