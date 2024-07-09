
#include <ansi.h>

inherit COMBINED_ITEM;

int value() { return query_amount() * (int)query("base_value"); }

void create()
{
	set_name(HIC "百合" NOR, ({"bai he", "bai"}));
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "这是一朵典雅孤傲的百合花，花的鳞茎乳白似玉，瓣瓣紧抱，\n"
			        "由二、三十瓣重叠累生 在一起，好象百片合成，隐含着“百\n"
					"年好合，百事合心”之意。\n");
		set("unit", "朵");
		set("value", 45000);
		set("base_unit", "朵");
        	set("base_weight", 100);
		set("flower_w",1);
		}
		set_amount(1);
}
#include "flower.h"