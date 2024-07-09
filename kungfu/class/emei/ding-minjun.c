// /kungfu/class/emei/ding-minjun.c
// Creat by Lklv 2001.10.18

inherit NPC;
inherit F_MASTER;
void create()
{
	set_name("丁敏君", ({"ding minjun", "ding", "minjun"}));
	set("long", "她是灭绝师太的八大俗家第子之一。\n");
	set("nickname","毒手无盐");
	set("gender", "女性");
	set("attitude", "friendly");
	set("unique", 1);
	set("age", 25);
	set("shen_type", 1);
	set("str", 30);
	set("int", 30);
	set("con", 30);
	set("dex", 30);
	set("per", 22);
	set("max_qi", 800);
	set("max_jing", 800);
	set("neili", 800);
	set("max_neili", 800);
	set("jiali", 8);
	set("combat_exp", 200000);
	set("score", 100);

	set_skill("dacheng-fofa", 130);
	set_skill("linji-zhuang", 140);
	set_skill("huifeng-jian", 120);
	set_skill("sword", 130);
	set_skill("literate", 130);
	set_skill("parry", 130);
	set_skill("dodge", 130);
	set_skill("force", 120);
	set_skill("anying-fuxiang", 130);

	map_skill("force", "linji-zhuang");
	map_skill("sword", "huifeng-jian");
	map_skill("parry", "huifeng-jian");
	map_skill("dodge", "anying-fuxiang");
        create_family("峨嵋派", 4, "弟子");

        setup();

	carry_object("/d/emei/obj/changjian")->wield();
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