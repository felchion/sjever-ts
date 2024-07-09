
#include <ansi.h>
#include <armor.h>

inherit NECK;

void create()
{
	set_name(MAG "紫色水晶项链" NOR, ({ "shuijing xianglian", "xianglian" }) );
	set_weight(1000);
	if (clonep()) set_default_object(__FILE__);
	else
	{
		set("long","紫水晶代表感情、智慧的提升，广结人缘，招福挡煞。\n");
		set("female_only",1);
		set("unit", "个");
		set("value", 10000);
		set("material", "jade");
		set("armor_prop/armor",1);
		set("no_get",1);
		set("no_drop",1);
		set("remove_msg","$N细心得将"+name()+"摘下，放在贴身处收藏。\n");
		set("wear_msg","$N将"+name()+"戴在自己颈上，得意非凡。\n");
	}
	setup();
}

int query_autoload()
{
	return 1;
}