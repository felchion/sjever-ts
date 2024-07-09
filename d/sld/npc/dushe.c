inherit NPC;

void create()
{
	set_name("毒蛇", ({ "du she", "dushe", "snake", "she" }));
	set("race", "野兽");
	set("age", 20);
	set("long", "一条黄中间黑的毒蛇，全身血红、长舌吞吐、嗤嗤作声、头作三角\n");
	set("attitude", "peaceful");

	set("limbs", ({ "头部", "身体", "七寸", "尾巴" }) );
	set("verbs", ({ "twist", "bite" }) );

	set("combat_exp", 20000);

	set_temp("apply/attack", 100);
	set_temp("apply/defense", 100);
	set_temp("apply/damage", 100);
	set_temp("apply/armor", 100);

	setup();
}

int hit_ob(object me, object victim, int damage)
{
	victim->apply_condition("snake_poison",
		victim->query_condition("snake_poison") + 8);
	return 0;
}


