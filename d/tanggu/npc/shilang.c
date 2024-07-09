
inherit NPC;

void create()
{
	set_name("施琅", ({ "shi lang", "shi" }));
	set("title", "水师提督");
	set("shen_type", 1);

	set("gender", "男性");
	set("age", 30);
	set("long", "这人就是大清水师提督，一身武将打扮威风凛凛。\n");

	set_skill("cuff", 120);
	set_skill("dodge", 120);
	set_skill("taizu-quan", 120);
	set_skill("xiaoyaoyou", 120);
	set_skill("force", 120);
	set_skill("huntian-qigong", 120);
	set_skill("parry", 120);
	set_skill("literate", 100);
	
	map_skill("cuff", "taizu-quan");
	map_skill("parry", "taizu-quan");
	map_skill("dodge", "xiaoyaoyou");
	map_skill("force", "huntian-qigong");
	prepare_skill("cuff", "taizu-quan");

	set_temp("apply/damage", 50);

	set("combat_exp", 300000);
	set("attitude", "friendly");
		
	setup();
	carry_object(ARMOR_D("armor"))->wear();
}
