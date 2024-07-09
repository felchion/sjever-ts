// xi.c 张松溪

inherit NPC;
inherit F_MASTER;

void create()
{
	set_name("张松溪", ({ "zhang songxi", "zhang" }));
	set("nickname", "武当四侠");
	set("long", 
		"他就是张三丰的四弟子张松溪。\n"
		"他今年四十岁，精明能干，以足智多谋著称。\n");
	set("gender", "男性");
	set("age", 40);
	set("attitude", "peaceful");
	set("shen", 8000);
	set("str", 26);
	set("int", 30);
	set("con", 26);
	set("dex", 28);
	set("unique", 1);
	set("max_qi", 1500);
	set("max_jing", 1200);
	set("neili", 2500);
	set("max_neili", 2500);
	set("jiali", 50);
	set("combat_exp", 800000);
	set("eff_jingli",2000);

	set_skill("force", 120);
        set_skill("yinyun-ziqi", 120);
	set_skill("dodge", 120);
	set_skill("tiyunzong", 120);
	set_skill("cuff", 120);
	set_skill("taiji-quan", 120);
	set_skill("parry", 120);
	set_skill("sword", 120);
	set_skill("taiji-jian", 120);
	set_skill("taoism", 100);
	set_skill("literate", 100);

        map_skill("force", "yinyun-ziqi");
	map_skill("dodge", "tiyunzong");
	map_skill("cuff", "taiji-quan");
	map_skill("parry", "taiji-quan");
	map_skill("sword", "taiji-jian");
	prepare_skill("cuff", "taiji-quan");

	create_family("武当派", 2, "弟子");

	setup();
        carry_object("/d/wudang/obj/white-robe")->wear();
	carry_object("/clone/weapon/changjian")->wield();
        carry_object("/d/wudang/obj/daolv")->wear();
}

void kill_ob(object me)
{
        set("chat_chance_combat", 20);
        set("chat_msg_combat", ({
           (: perform_action, "sword.chan" :),
           (: perform_action, "dodge.zong" :),
           (: perform_action, "cuff.zhen" :),
        }));
        ::kill_ob(me);       
}

void attempt_apprentice(object ob)
{
       mapping fam = ob->query("family");
       if (!fam || fam["family_name"] != "武当派"){
                command("say "+RANK_D->query_respect(ob)+"与本派素无来往，不知此话从何谈起？");
                return;
       }
       if ((int)ob->query("shen") < 10000) {
	       command("say 我武当乃是堂堂名门正派，对弟子要求极严。");
	       command("say 在德行方面，" + RANK_D->query_respect(ob) +
		       "是否还做得不够？");
	       return;
       }
       if ((int)ob->query_skill("yinyun-ziqi", 1) < 80) {
                command("say 我武当派乃内家武功，最重视内功心法。"); 
                command("say " + RANK_D->query_respect(ob) + 
                        "是否还应该在氤氲紫气上多下点功夫？");
                return;
       }
       if (ob->query_int() < 28) {
                command("say 我武当派武功全从道藏悟出。");
                command("say 要能达到炉火纯青之境，体质什么的倒是无关紧要，悟性却是半点也马虎不得。");
                command("say " + RANK_D->query_respect(ob) + "的悟性还大有潜力可挖，还是请回吧。");
                return;
       }
       command("say 好吧，贫道就收下你了。");
       command("recruit " + ob->query("id"));
}
