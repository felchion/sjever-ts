inherit ITEM;
inherit F_LIQUID;

void create()
{
	set_name("瓷茶碗", ({"ci chawan", "chawan","ci"}));
	set_weight(100);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "一个蓝边粗磁大碗。\n");
		set("unit", "个");
		set("value", 100);
		set("max_liquid", 4);
	}

    // because a container can contain different liquid
    // we set it to contain tea at the beginning
	set("liquid", ([
		"type": "tea",
		"name": "茶水",
		"remaining": 4,
		"drunk_supply": 0,
	]));
}
