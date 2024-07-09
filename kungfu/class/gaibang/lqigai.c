// znqigai.c 中年乞丐

inherit NPC;
#include "begger.h";

void create()
{
	set_name("老乞丐", ({ "qi gai", "di zi", "qigai" }));
	set("title","丐帮帮众");
	set("gb/bags", 1);
	set("long","这是一个老乞丐，脸上已有不少皱纹。\n");
	set("gender", "男性");
	set("age", 50+random(30));
	set("attitude", "peaceful");
	set("shen_type", 0);
	set("str", 20);
	set("int", 16);
	set("con", 22);
	set("dex", 24);

	set("max_qi", 300);
	set("max_jing", 300);
	set("eff_jingli", 500);
	set("max_neili", 300);
	set("jiali", 10);
	set("combat_exp", 3000);

	set_skill("force", 30);
	set_skill("dodge", 30);
	set_skill("unarmed", 30);
	set_skill("parry", 30);
	set_skill("begging", 30);

	create_family("丐帮", 20, "弟子");
	setup();
	carry_object(CLASS_D("gaibang/obj/gb_budai1"))->wear();
	carry_object(ARMOR_D("bainayi"))->wear();
	carry_object(ARMOR_D("caoxie"))->wear();
}
