// /d/dzd/obj/mujian.c

#include <ansi.h>
#include <weapon.h>

inherit SWORD;
//inherit F_UNIQUE;

void create()
{
	set_name("檀香木剑",({"tanxiang mujian","mujian","jian","sword"}));
	set_weight(1000);
	if(clonep())
		set_default_object(__FILE__);
	else {
		set("unit","柄");
		set("value",1000);
		set("meterial","wood");
		set("long","这是一把由奇鲮香木制成的木剑，剑刃淡黄，隐隐闻到一股淡淡的香气。\n");
		set("wield_msg",HIY"$N轻飘飘的抽出一把檀香木剑，随即一股淡淡的香气在四周弥漫开来。\n"NOR);
		set("unwield_msg",HIY"$N轻轻的将剑插入鞘中，香气也慢慢的淡了。\n"NOR);
	}
	init_sword(100);
	setup();
}
