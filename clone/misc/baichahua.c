// baichahua.c 白茶花
// creat by Lklv@SJ for 各位美女 @@

#include <ansi.h>
#include <armor.h>
inherit HEAD;

void create()
{
	set_name(HIW"白茶花"NOR, ({"bai chahua", "chahua", "hua"}));
	set_weight(300);
	if( clonep() )
		set_default_object(__FILE__);
        else {
        	set("unit","朵");
		set("value",150);
		set("female_only", 1);
        	set("armor_prop/armor", 1);
		set("long", "这是一束白茶花，颜色洁白，象纯洁的少女。\n");
		set("wear_msg", "$N拿起一朵"HIW"白茶花"NOR"插在发边。\n");
		set("remove_msg", "$N摘下发边的"HIW"白茶花"NOR"，小心翼翼的把它收好。\n"NOR);
	}
        setup();
}
