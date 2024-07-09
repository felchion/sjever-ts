
#include <ansi.h>
#include <armor.h>

inherit HEAD;

#include "token.h"

void create()
{	
	set_name(HIY"黄金头饰"NOR, ({ "headgear" }) );
	set_weight(100);
	set("long","用"HIY"黄金"NOR"制成的头饰,看上去精巧无比。\n在它的内侧刻着一行铭文:\n");
	set("unit", "个");
	set("value", 3000000);
	set("no_get",1);
	set("no_drop",1);
	set("armor_type", "token_head");
	set("armor_prop/armor",25);
	setup();

}


void init()
{
	add_action("do_discard","discard");
	token_reload();
}

