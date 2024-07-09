
#include <ansi.h>
#include <armor.h>

inherit WRISTS;

#include "token.h"

void create()
{	
	set_name(HIG"翡翠玉镯"NOR, ({ "bangle" }) );
	set_weight(100);
	set("long","这是用上好的缅甸"HIG"翡翠"NOR"精雕细琢而成的玉镯,通体晶莹剔透让人爱不释手。\n在它的内侧还隐约可以看到一行小字:\n");
	set("unit", "只");
	set("value", 3000000);
	set("no_get",1);
	set("no_drop",1);
	set("armor_type", "token_hand");
	set("armor_prop/armor",25);
	setup();

}

void init()
{
	add_action("do_discard","discard");
	token_reload();
}