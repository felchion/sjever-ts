// gaozecheng.c (kunlun)

inherit NPC;
inherit F_MASTER;

void create()
{
	set_name("苏习之", ({ "su xizhi", "su" }));
	set("long", "他三十多岁年纪。\n");
	set("gender", "男性");
	set("age", 32);
	set("attitude", "heroism");
	set("shen_type", 1);
	set("str", 25);
	set("int", 25);
	set("con", 25);
	set("dex", 25);

	set("max_qi", 600);
	set("max_jing", 500);
	set("neili", 500);
	set("max_neili", 500);
	set("jiali", 30);
	set("combat_exp", 150000);
	set("score", 10000);

	set_skill("force", 80);
	set_skill("xuantian-wuji", 85);
	set_skill("dodge", 80);
	set_skill("taxue-wuhen", 90);
	set_skill("parry", 80);
	set_skill("sword", 85);
	set_skill("xunlei-jian", 95);
	set_skill("cuff", 80);
	set_skill("zhentian-quan", 90);

	map_skill("force", "xuantian-wuji");
	map_skill("dodge", "taxue-wuhen");
	map_skill("sword", "xunlei-jian");
	map_skill("cuff", "zhentian-quan");
	prepare_skill("cuff", "zhentian-quan");

	create_family("昆仑派", 4, "弟子");
	setup();
        carry_object(BINGQI_D("changjian"))->wield();
        carry_object("/d/kunlun/obj/pao3")->wear();
}

void attempt_apprentice(object ob)
{

	command("say 我师傅不准我收弟子，你去找我师姊去吧。");
	return;
}