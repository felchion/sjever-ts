#include <weapon.h>
#include <ansi.h>

inherit WHIP;
inherit F_UNIQUE;

void create()
{
	set_name("黑索",({ "hei suo", "whip","bian","suo" }) );
	set_weight(15000);
	if( clonep() )
	set_default_object(__FILE__);
	else {
		set("unit", "条");
		set("value", 1);
		set("unique", 3);
		set("rigidity", 7);
		set("material", "steel");
		set("weapon_prop/parry", 4);
		set("treasure",1);
		set("wield_neili", 900);
		set("wield_maxneili", 1400);
		set("wield_str", 24);
		set("weapon_prop/ap", 80);
		set("long", "这是条通体黝黑无光的长索，便如一条张牙舞爪的墨龙，夜晚舞动之时瞧不见半点影子。\n");
		set("unwield_msg", "$N随手一抖，将$n盘回腰间。\n");
		set("wield_msg", "$N随手一抖，抽出一条$n握在手中。\n");
	}
	init_whip(150);
	setup();
}
