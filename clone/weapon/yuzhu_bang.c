// bamboo_stick.c 玉竹棒
// by shang 97/6
// modified by explorer, 9/11/97

#include <ansi.h>
#include <weapon.h>

inherit STICK;

void create()
{

	set_name(HIG "玉竹棒" NOR, ({"yuzhu bang", "bang", "stick"}));
	set_weight(5000);

	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "根");
		set("long",HIG + "一根看起来温润晶莹，通体碧绿的竹棒，泛着淡淡的青光 。\n" NOR);
		set("value", 10000);
		set("material", "bamboo");
		set("weight", 10000);
		set("treasure", 1);
		set("unique", 1);
		set("weapon_prop/parry", 3);
		set("rigidity", 9);
		set("weapon_prop/ap", 160);
		set("wield_msg", "$N抽出一根青光闪闪的玉竹杖抓在手中。\n");
		set("unwield_msg", "$N把手中的玉竹杖插回腰中。\n");
	}
	init_stick(160);
	setup();
}

mixed hit_ob(object me, object victim, int damage_bonus)
{
	if (!victim->is_busy() && !random(6)) {
		victim->start_busy(random(me->query_skill("strik",1) / 300) + 1);
		return HIM "突然间玉竹棒上泛出一道青光，$n顿时只觉得两眼隐隐刺痛！\n" NOR;
	}
	return damage_bonus;
}