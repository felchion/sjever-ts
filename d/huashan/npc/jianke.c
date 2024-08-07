// NPC: /d/huashan/npc/jianke.c
// Date: Look 99/03/25

#include <ansi.h>

inherit NPC;

void create()
{
	set_name("剑客", ({ "jian ke","ke" }) );
	set("gender", "男性");
	set("age", 25 + random(20));
	set("long",
		"这是位中年武人，肩背长剑，长长的剑穗随风飘扬，看来似乎身怀绝艺。\n");
	set("combat_exp", 20000);
       set("shen_type", -1);
	set("attitude", "heroism");

	set("apply/attack",  30);
	set("apply/defense", 30);

	set_skill("sword", 60);
	set_skill("unarmed", 50);
	set_skill("parry", 50);
	set_skill("dodge", 50);

	setup();
	carry_object(BINGQI_D("sword/sword"))->wield();
	carry_object(ARMOR_D("armor/cloth"))->wear();
	add_money("silver", 20 + random(20));
}

