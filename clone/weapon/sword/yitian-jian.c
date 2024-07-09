// yitian-jian 倚天剑

#include <weapon.h>
#include <ansi.h>

inherit SWORD;
inherit F_UNIQUE;

void create()
{
	set_name(HIC"倚天剑"NOR,({ "yitian jian", "yitian", "sword", "jian" }) );
	set_weight(20000);
	if( clonep() )
	set_default_object(__FILE__);
	else {
		set("unit", "把");
		set("value", 1);
		set("rigidity", 10);
		set("sharpness", 9);
		set("unique", 1);
		set("material", "steel");
		set("weapon_prop/parry", 8);
		set("treasure",1);
		set("wield_neili", 1300);
		set("wield_maxneili", 1700);
		set("wield_str", 28);
		set("weapon_prop/ap", 100);
		set("wield_msg", HIY"$N“唰”的一声抽出倚天剑挽了个剑花，但见寒芒吞吐，电闪星飞。\n$N不由得从心底升起一股凌云豪气。\n"NOR);
		set("long", "这是一柄四尺来长的古剑，剑鞘上金丝镶着‘倚天’两个篆文。\n整个剑身隐隐发出一层青气，剑未出鞘，已可见其不凡。\n");
		set("unwield_msg", HIY "倚天剑幻作一道白光，「唰」地飞入剑鞘。\n" NOR);
	}
	init_sword(200);
	setup();
}

mixed hit_ob(object me, object victim, int damage_bonus)
{
	int n;


	if (me->query_skill_mapped("sword") != "huifeng-jian" ||
	me->query_skill("huifeng-jian", 1) < 100)
	return damage_bonus / 2;

	switch (random(8))
	{
		case 0:
		if (! victim->is_busy())
		victim->start_busy(random(me->query_skill("sword") / 300) + 1);
		return WHT "倚天剑携着刺骨风声划破天际，$n只觉全身力气竟一丝丝随着剑锋涌出。\n"NOR;



		case 1:
		n = me->query_skill("sword");
		victim->receive_damage("qi", n , me);
		victim->receive_wound("qi", n/3 , me);
		victim->receive_damage("neili", n , me);

		return     WHT "倚天剑夹带着无数道剑气凌厉的袭向$n,$n只觉得阴柔的真气攻入自己的经脉。\n" NOR;


	}
	return damage_bonus;
}
