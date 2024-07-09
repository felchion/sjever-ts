///u/jpei/thd/obj/biyuxiao.c 碧玉箫
#include <weapon.h>
#include <ansi.h>

inherit SWORD;

void create()
{
	set_name(GRN"碧玉箫"NOR,({"biyu xiao","xiao"}));
	set_weight(9000);
	if (clonep())
	set_default_object(__FILE__);
	else{
		set("unit","支");
		set("value",2000);
		set("material","gem");
		set("unique", 1);
		set("treasure", 1);
		set("rigidity", 6);
		set("weapon_prop/parry", 5);
		set("wield_neili", 500);
		set("wield_maxneili", 1000);
		set("weapon_prop/ap", 60);
		set("long","这是一支通体晶莹的用极为罕见的"GRN"碧玉"NOR"做的箫，看起来年代十分久远。\n");
		set("wield_msg","$N伸手轻轻一招，一支$n已出现在$N手中。\n");
		set("unwield_msg","$N一声清啸，将$n插回腰间。\n");
	}
	init_sword(120);
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

	switch (random(12))
	{
		case 0:
		if (! victim->is_busy())
		victim->start_busy(random(me->query_skill("sword") / 300) + 1);
		return HIM "突然间一段【碧海潮生曲】至$N的箫中传来，$n顿时只觉得太阳穴隐隐刺痛！\n" NOR;
		case 1:
		if (! victim->is_busy())
		victim->start_busy(random(me->query_skill("sword") / 300) + 1);
		return HIM "突然间一段【碧海潮生曲】至$N的箫中传来，$n顿时只觉得太阳穴隐隐刺痛！\n" NOR;
		case 2:
		n = me->query_skill("sword");
		victim->receive_damage("qi", n /2, me);
		victim->receive_wound("qi", n / 5, me);
		victim->receive_damage("neili", n , me);
		return HIY "$N手中玉箫随招式激荡出阵阵箫音，$n顿时只觉气血上涌，难受之极！\n" NOR;
	}
	return damage_bonus;
}

