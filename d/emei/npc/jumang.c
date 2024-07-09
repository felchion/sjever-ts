// animal: snake.c
// Jay 3/18/96

inherit NPC;

void create()
{
	set_name("巨蟒", ({ "ju mang", "mang" }));
	set("race", "野兽");
	set("age", 200);
	set("long", "一条庞大无比, 色彩斑斓的巨蟒。 浑身发出阵阵强烈的腥臭味。\n");
	set("attitude", "aggressive");
	set("str", 70);
	set("con", 50);
	set("max_qi", 800);
	set("no_get","巨蟒对你而言太重啦，你无法拿动");
	set("limbs", ({ "头部", "身体", "尾巴" }) );
	set("verbs", ({ "bite" }) );
	set("combat_exp", 200000);
	set_temp("apply/attack", 50);
	set_temp("apply/damage", 50);
	set_temp("apply/armor", 50);
	setup();
	if (clonep() && random(3)==1) carry_object(MEDICINE_D("renshen-guo"));
}

void die()
{
	object ob;
	message_vision("$N抽搐了几下，身体缩在一起，死了。\n", this_object());
	ob = new(__DIR__"msdan");
	ob->move(environment(this_object()));
	destruct(this_object());
}
