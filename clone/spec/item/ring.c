
#include <ansi.h>
#include <armor.h>

inherit FINGER;

#include "token.h"

void create()
{	
	set_name(HIW"白金指环"NOR, ({ "ring" }) );
	set_weight(100);
	set("long","一个"HIW"白金"NOR"制成的指环,还镶着一颗硕大的"HIW"钻石"NOR"闪耀着引人夺目的光彩。\n在它的内圈刻着一行铭文:\n");
	set("unit", "个");
	set("value", 3000000);
	set("no_get",1);
	set("no_drop",1);
	set("armor_type", "token_finger");
	set("armor_prop/armor",25);
	setup();

}

void init()
{
	add_action("do_discard","discard");
	token_reload();
}