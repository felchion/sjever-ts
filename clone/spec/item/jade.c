
#include <ansi.h>
#include <armor.h>

inherit WAIST;

#include "token.h"

void create()
{	
	set_name(HIR"龙凤玉佩"NOR, ({ "jade" }) );
	set_weight(100);
	set("long","这一块用极为罕见的"HIR"赤霞玉"NOR"制成玉佩,上面雕刻着一对栩栩如生的腾龙飞凤。\n在它的背面有一行铭文:\n");;
	set("unit", "块");
	set("value", 3000000);
	set("no_get",1);
	set("no_drop",1);
	set("armor_type", "token_waist");
	set("armor_prop/armor",25);
	setup();

}

void init()
{
	add_action("do_discard","discard");
	token_reload();
}