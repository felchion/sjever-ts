// yu.c 俞莲舟

inherit NPC;
inherit F_MASTER;

void create()
{
        set_name("俞莲舟", ({ "yu lianzhou", "yu" }));
        set("nickname", "武当二侠");
        set("long", 
                "他就是张三丰的二弟子俞莲舟。\n"
                "他今年五十岁，身材魁梧，气度凝重。\n"
                "虽在武当七侠中排名第二，功夫却是最精。\n");
        set("gender", "男性");
        set("age", 50);
        set("attitude", "peaceful");
        set("shen", 12000);
        set("str", 28);
        set("int", 28);
        set("con", 28);
        set("dex", 28);
        set("unique", 1);
        set("max_qi",5000);
        set("eff_jingli",2500);
        set("max_jing", 1500);
        set("neili",5000);
        set("max_neili", 2500);
        set("jiali", 150);
        set("combat_exp",2000000);

        set_skill("force", 140);
        set_skill("yinyun-ziqi", 140);
        set_skill("dodge",180);
        set_skill("tiyunzong",180);
        set_skill("wudang-quan",180);
        set_skill("cuff",180);
        set_skill("hand",180);
        set_skill("taiji-quan", 150);
        set_skill("juehu-shou",180);
        set_skill("parry", 150);
        set_skill("sword", 150);
        set_skill("taiji-jian", 140);
        set_skill("taoism", 140);
        set_skill("literate", 100);

        map_skill("force", "yinyun-ziqi");
        map_skill("dodge", "tiyunzong");
        map_skill("parry", "taiji-quan");
        map_skill("sword", "taiji-jian");
        map_skill("cuff", "wudang-quan");
        map_skill("hand", "juehu-shou");

        prepare_skill("cuff", "wudang-quan");
        prepare_skill("hand", "juehu-shou");

        create_family("武当派", 2, "弟子");

        setup();
//      carry_object("/clone/weapon/changjian")->wield();
        carry_object("/d/wudang/obj/whiterobe")->wear();
        carry_object("/d/wudang/obj/daolv")->wear();
}

void kill_ob(object me)
{
        set("neili", 5000); 
        set("chat_chance_combat", 60);
        set("chat_msg_combat", ({
              (: perform_action, "dodge.zong" :),
        }));
        ::kill_ob(me);
}

void attempt_apprentice(object ob)
{
        object me;
        me=this_player();
        if ((int)ob->query("shen") < 10000) {
                command("say 我武当乃是堂堂名门正派，对弟子要求极严。");
                command("say " + RANK_D->query_respect(ob) + "若能做出" +
                   "几件侠义之事，我就考虑收你。");
                return;
        }
	if ((int)ob->query_skill("taoism",1) < 100) {
                command("say 你的道学修为不够，只怕你跟我学了三阴绝户手后出去害人！");
                return;
        }
	if (ob->query("gender") != "男性" && ob->query("gender") != "女性") {
                command("say 你阴阳不调，还想跟我学绝户手？");
                return;
        }
        if (present("letters", me)){
        	destruct(present("letters",me));
                command("say 好吧，既然" + RANK_D->query_respect(ob) + "有俞师弟的介绍信，也是" +
                        "我辈中人，今天就收下你吧。");
                command("recruit " + ob->query("id"));
        }
        else
                command("say 我对弟子要求极严。非心地善良者不能学我三阴绝户手!");
        return;
          
}

int do_kill(string arg)
{
        object ob,me = this_player();

        if (!living(this_object())) return 0;
        if (!arg) return 0;
        if (!(ob = present(arg, environment()))) return 0;
        if (ob == me) return 0;
        if (ob->query("family/family_name") != "武当派") return 0;
        
        command("say 大胆狂徒，竟敢在武当放肆！！！");
        command("stare "+this_player()->query("id"));
        message_vision("俞莲舟哈哈大笑几声！\n", this_player());
        kill_ob(me);
        return 1;
}

void init()
{
        ::init();
        if (query("startroom") != base_name(environment())) return;
        add_action("do_kill", "kill");
        add_action("do_kill","hit");
}
