// jgxmc.c 
#include <ansi.h>
#include <weapon.h>
inherit STAFF;
inherit F_UNIQUE;
void create()
{
	set_name( HIY"金刚降魔杵"NOR , ({ "jingang chu", "jingangchu","chu","staff"}));
	set_weight(70000);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "根");
		set("long", HIR"这根法杵约有一丈一二尺长，入手非常沉重，上面刻满梵文，杵身罩在一层隐隐的红光之中。\n"NOR);
		set("value", 1000);
		set("unique",1);
		set("rigidity", 7);
		set("treasure",1);
		set("wield_neili", 1200);
		set("wield_maxneili", 1600);
		set("wield_str", 30);
		set("weapon_prop/parry", 7);
		set("weapon_prop/ap", 120);
		set("material", "gold");
		set("wield_msg", HIY"$N伸手在袍底拿出金刚降魔杵，轻轻一舞，映得金光满地。\n"NOR);
		set("unwield_msg",HIR" $N把金刚降魔杵小心的放入袍中。\n"NOR);
	}
	init_staff(160);
	setup();
}
