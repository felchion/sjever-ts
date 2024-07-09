// 9xie.c 九耳麻鞋
#include <ansi.h>
#include <armor.h>

inherit BOOTS;

void create()
{
	set_name( HIY "九耳麻鞋" NOR, ({ "jiuer maxie", "maxie","xie","shoe" }) );
	set_weight(600);
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("unit", "双");
		set("long", "由草藤编织而成，既轻便又结实，前后织有九环，形状象耳朵。");
		set("value", 300);
		set("material", "boots");
		set("armor_prop/dodge", 5);
	}
	setup();
}
