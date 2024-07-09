// liao_kao.c

#include <ansi.h>
#include <armor.h>

inherit BANGLE;

void create()
{
	set_name(HIG "龙凤玉镯" NOR, ({ "longfeng yuzhuo", "yuzhuo" }) );
	set_weight(1000);
	if (clonep()) set_default_object(__FILE__);
	else
	{
		set("long","上面雕有栩栩如生的龙凤图案，是祝愿新人成双成对，白头偕老的首饰经典。\n");
		set("female_only",1);
		set("unit", "个");
		set("value", 10000);
		set("material", "jade");
		set("armor_prop/armor",1);
		set("no_get",1);
		set("no_drop",1);
		set("remove_msg","$N细心得将"+name()+"摘下，放在贴身处收藏。\n");
		set("wear_msg","$N将"+name()+"戴在自己手腕上，得意非凡。\n");
	}
	setup();
}

int query_autoload()
{
	return 1;
}