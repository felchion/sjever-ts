// juhua.c 菊花
// creat by Lklv@SJ for 各位美女 @@

#include <ansi.h>
#include <armor.h>
inherit HEAD;

void create()
{
	set_name(HIY"菊花"NOR, ({"ju hua", "juhua", "hua"}));
	set_weight(300);
	if( clonep() )
		set_default_object(__FILE__);
        else {
        	set("unit","朵");
		set("value",150);
		set("female_only", 1);
        	set("armor_prop/armor", 1);
		set("long", "这是一朵美丽的菊花，象纯洁的少女。\n");
		set("wear_msg", "$N拿起一朵$n插在发边。\n");
		set("remove_msg", "$N摘下发边的$n，小心翼翼的把它收好。\n"NOR);
	}
        setup();
}
