// zhanchun.c (kunlun)

inherit NPC;
inherit F_MASTER;

void create()
{
	set_name("詹春", ({ "zhan chun", "zhan", "chun" }));
	set("long",
		"她是何太冲最喜欢的弟子，长得倒也颇为俏丽。\n");
	set("gender", "女性");
	set("age", 24);
	set("attitude", "friendly");
	set("shen_type", 1);
	set("str", 25);
	set("int", 24);
	set("con", 28);
	set("dex", 26);

	set("eff_jingli", 850);
	set("max_qi", 850);
	set("max_jing", 600);
	set("neili", 1200);
	set("max_neili", 1200);
	set("jiali", 30);
	set("combat_exp", 380000);
	set("score", 4000);

	set_skill("force", 95);
	set_skill("xuantian-wuji", 105);
	set_skill("dodge", 95);
	set_skill("taxue-wuhen", 105);
	set_skill("parry", 100);
	set_skill("sword", 101);
	set_skill("xunlei-jian", 105);
	set_skill("leg", 90);
	set_skill("chuanyun-tui", 100);
	set_skill("throwing", 80);
	set_skill("mantian-huayu", 95);


	map_skill("force", "xuantian-wuji");
	map_skill("dodge", "taxue-wuhen");
	map_skill("sword", "xunlei-jian");
	map_skill("leg", "chuanyun-tui");
        map_skill("sword", "xunlei-jian");

        prepare_skill("leg", "chuanyun-tui");

        create_family("昆仑派", 4, "弟子");
	setup();
	carry_object(BINGQI_D("changjian"))->wield();
	carry_object("/d/kunlun/obj/pao3")->wear();
}

void attempt_apprentice(object ob)
{
        if(ob->query("gender")=="男性"){
	   command("fear");
	   command("say "+RANK_D->query_respect(ob)+"师傅不准我收男弟子，你去找我师兄去吧。");
   	   return;
	 }
         if(ob->query_skill("xuantian-wuji",1)<60){
	   command("hmm");
	   command("say "+RANK_D->query_respect(ob)+"你的玄天无极功太差，我可不想收你。");
 	   return;
	 }
	 if(ob->query("appren_hezudao", 1) == 1) {
	   command("say 前辈怎敢开这等玩笑，真是折杀做晚辈的了。");
	   return;
	 }
	 command("say 既然"+RANK_D->query_respect(ob)+"这么努力，我就收下你吧。");
	 command("recruit " + ob->query("id"));
}
