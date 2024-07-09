
#include <ansi.h>
#include <armor.h>

inherit NECK;

#include "token.h"

void create()
{	
	set_name(HIM"紫晶项链"NOR, ({ "necklace" }) );
	set_weight(100);
	set("long","一条"HIW"白金"NOR"制成的项链,上面还挂着一颗"HIM"紫水晶"NOR"坠子,给人一种很神秘感觉。\n坠子背面刻着一行小字:\n");
	set("unit", "条");
	set("value", 3000000);
	set("no_get",1);
	set("no_drop",1);
	set("armor_type", "token_neck");
	set("armor_prop/armor",25);
	setup();

}

void init()
{
	add_action("do_discard","discard");
	token_reload();
}