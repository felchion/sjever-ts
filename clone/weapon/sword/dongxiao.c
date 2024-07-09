#include <weapon.h>
#include <ansi.h>

inherit SWORD;

void create()
{
	set_name(HIW"洞箫"NOR,({"dongxiao",  "xiao"}));
	set_weight(10000);
	if (clonep())
	set_default_object(__FILE__);
	else{
		set("unit", "支");
		set("value", 9000);
		set("rigidity", 9);
		set("sharpness", 5);
		set("material", "starsteel");
		set("treasure",1);
		set("unique",1);
		set("wield_neili", 500);
		set("wield_maxneili", 900);
		set("weapon_prop/parry", 2);
		set("weapon_prop/ap", 80);
		set("wield_msg",HIY"$N伸手轻轻一招，一支$n" + HIY"已出现在$N手中。\n"NOR);
		set("unwield_msg",HIY"$N一声清啸，将$n" + HIY"插回腰间。\n"NOR);
		set("long","这是一支通体雪白的"HIW"洞箫"NOR"，不知道是什么材质做成的。\n");
	}
	init_sword(160);
	setup();
}

int wield()
{
	object me = environment();
	if (living(me) && me->query_skill("yuxiao-jian", 1) < 200){
		message_vision("$N拿起"+name()+"，但是不会使用。\n", me);
		return 0;
	}
	if(!query("weapon_prop/damage")) return 0;
	return ::wield();
}

mixed hit_ob(object me, object victim, int damage_bonus)
{
	int n;

	switch (random(6))
	{
		case 0:
		if (! victim->is_busy())
		victim->start_busy(random(me->query_skill("sword") / 200) + 1);
		return HIM "突然间一段【碧海潮生曲】至$N的"+query("name")+HIM"中传来，$n顿时只觉得太阳穴隐隐刺痛！\n" NOR;
		case 1:
		n = me->query_skill("sword");
		victim->receive_damage("qi", n , me);
		victim->receive_wound("qi", n / 3, me);
		victim->receive_damage("neili", n , me);
		return HIY "$N手中玉箫随招式激荡出阵阵箫音，$n顿时只觉气血上涌，难受之极！\n" NOR;
	}
	return damage_bonus;
}