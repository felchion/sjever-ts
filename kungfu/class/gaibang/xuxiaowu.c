// xuxiaowu.c 徐小五

inherit NPC;

void create()
{
        set_name("徐小五", ({ "xu xiaowu", "xu", "xiaoxu" }));
        set("title","丐帮四袋弟子");
        set("long","这是一个丐帮弟子。\n");

        set("gender", "男性");
        set("age", random(20)+30);
        set("attitude", "peaceful");
        set("shen_type", 0);
        set("str", 18+random(5));
        set("int", 14+random(5));
        set("con", 20+random(5));
        set("dex", 22+random(5));

        set("max_qi", 2000+random(500));
        set("max_jing", 2000+random(200));
        set("neili", 3500+random(500));
        set("max_neili", 3000+random(500));
        set("max_jingli",2000+random(500));
        set("eff_jingli",2500+random(500));
        set("jiali", 100);
        set("combat_exp", 800000+random(200000));

        set_skill("force", 150);
        set_skill("dodge", 150);
        set_skill("parry", 150);
        set_skill("xianglong-zhang", 150);
        set_skill("xiaoyaoyou", 150);
        set_skill("huntian-qigong", 150);

        map_skill("force", "huntian-qigong");
        map_skill("strike", "xianglong-zhang");
        map_skill("dodge", "xiaoyaoyou");
        map_skill("parry", "xianglong-zhang");
        prepare_skill("strike", "xianglong-zhang");

        set("chat_chance_combat", 80);
        set("chat_msg_combat", ({
                (: perform_action, "strike.xiao" :),
                (: exert_function, "huntian" :),
                (: perform_action, "strike.paiyun" :),
                (: perform_action, "strike.kanglong" :),
        }));
        create_family("丐帮", 19, "四袋弟子");
        set("inquiry", ([
                "丐帮" : "我们丐帮是天下第一大帮！\n",
                "乔峰" : "乔帮主是个好汉子。\n",
                "萧峰" : "他是我们的前任帮主。\n",
        ]));
        setup();
        carry_object(CLASS_D("gaibang/obj/gb_budai4"))->wear();
        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
}
