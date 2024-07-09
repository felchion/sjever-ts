// junzi jian 君子剑
// Modify By River
#include <weapon.h>
#include <ansi.h>
inherit SWORD;
inherit F_UNIQUE;

void create()
{
	set_name(HIW"君子剑"NOR,({ "junzi jian", "junzi", "jian", "sword" }) );
	set_weight(12000);
	if( clonep() )
	set_default_object(__FILE__);
	else {
		set("unit", "柄");
		set("value", 1500);
		set("rigidity", 3);
		set("sharpness", 3);
		set("unique", 1);
		set("treasure", 1);
		set("material", "steel");
		set("weapon_prop/parry", 2);
		set("wield_neili", 500);
		set("wield_maxneili", 900);
		set("wield_str", 22);
		set("weapon_prop/ap", 40);
		set("long", "此剑乌黑的剑身，没半点光泽，就似一段黑木。刻着君子二字。与淑女剑一般长短，全无二致。\n");
		set("wield_msg", HIW"$N缓缓抽出君子剑，只觉轻飘飘无甚份量，到是一阵凉意凛然逼人，令人莫可忽视。\n"NOR);
		set("unwield_msg", HIW"$N作个剑势，将君子剑缓缓插回剑鞘，当真气度非凡。\n" NOR);
	}
	init_sword(120);
	setup();
}
