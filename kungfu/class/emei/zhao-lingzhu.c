// /kungfu/class/emei/zhao-lingzhu.c
// Update by Lklv 2001.10.18 兼收男弟子

inherit F_MASTER;
inherit NPC;

void create()
{
	set_name("赵灵珠", ({"zhao lingzhu", "zhao", "lingzhu"}));
	set("long", "她是灭绝师太八大俗家弟子之一。\n");
	set("gender", "女性");
	set("attitude", "friendly");
	set("unique", 1);

	set("age", 30);
	set("shen_type", 1);
	set("str", 27);
	set("int", 27);
	set("con", 27);
	set("dex", 27);

	set("max_qi", 1000);
	set("max_jing", 1000);
	set("neili", 1000);
	set("max_neili", 1000);
	set("jiali", 25);
	set("combat_exp", 200000);
	set("score", 100);

	set_skill("dacheng-fofa", 90);
	set_skill("linji-zhuang", 80);
	set_skill("huifeng-jian", 80);
	set_skill("sword", 80);
	set_skill("parry", 90);
	set_skill("dodge", 90);
	set_skill("literate", 90);
	set_skill("force", 90);
	set_skill("anying-fuxiang", 90);
	set_skill("yanxing-daofa", 80);
	set_skill("blade", 80);

	map_skill("force", "linji-zhuang");
	map_skill("sword", "huifeng-jian");
	map_skill("parry", "huifeng-jian");
	map_skill("dodge", "anying-fuxiang");
	map_skill("blade", "yanxing-daofa");

	create_family("峨嵋派", 4, "弟子");
	setup();
	carry_object(BINGQI_D("changjian"))->wield();
	carry_object("/d/emei/obj/shoes")->wear();
	carry_object("/d/emei/obj/cloth")->wear();
}

void attempt_apprentice(object ob)
{
	if ((string)ob->query("class") == "bonze" ){
		command ("say 我只收俗家弟子。");
		return;
	}
	if ((string)ob->query("gender") == "无性" ){
		command ("angry ");
		command ("say 我最讨厌的就是不男不女之人。");
		return;
	}
	command (":) ");
	command ("say 好吧！我就收下你做峨嵋派俗家弟子。");
	command("recruit " + ob->query("id"));
	if( (string)ob->query("class")!="bonze" ){
		ob->set("title", "峨嵋派俗家弟子");
//		ob->set("class","emsujia");
	}
}