// /kungfu/class/nshaolin/da-dian.c
// looklove 2001.7.21

inherit NPC;


void create()
{
        set_name("大癫大师", ({
                "dadian dashi",
                "dadian",
                "dashi",
        }));
        set("long",
                "他看起来天生神武，虎背熊腰，身材高出常人一头。\n"
        );

        set("gender", "男性");
        set("attitude", "friendly");
        set("class", "bonze");
	set("title", "戒持院首座");

        set("age", 52);
        set("shen_type", 100);
        set("str", 40);
        set("int", 30);
        set("con", 30);
        set("dex", 30+random(10));
        set("max_qi", 3300);
        set("max_jing", 3000);
        set("neili", 8500);
        set("max_neili", 5000);
        set("jiali", 50);
        set("combat_exp", 2200000);

        set_skill("force", 240);
        set_skill("yijin-jing", 240);
        set_skill("dodge", 240);
        set_skill("shaolin-shenfa", 240);
        set_skill("parry", 240);
        set_skill("claw", 240);
        set_skill("staff", 240);
        set_skill("finger", 240);
        set_skill("longzhua-gong", 240);
        set_skill("wuchang-zhang", 240);
        set_skill("yizhi-chan", 240);
        set_skill("medicine", 120);
        set_skill("buddhism", 200);
        set_skill("literate", 140);

        map_skill("force", "yijin-jing");
        map_skill("dodge", "shaolin-shenfa");
        map_skill("claw", "longzhua-gong");
        map_skill("finger", "yizhi-chan");
        map_skill("staff", "wuchang-zhang");
        map_skill("parry", "yizhi-chan");

        prepare_skill("claw", "longzhua-gong");
        prepare_skill("finger", "yizhi-chan");

	set("chat_chance_combat", 60);
	set("chat_msg_combat", ({
		(: perform_action, "finger.qiankun" :),
		(: perform_action, "finger.wofo" :),
		(: perform_action, "claw.canyun" :),
	}));

        create_family("南少林", 22, "弟子");

        setup();

	carry_object("/d/shaolin/obj/chanzhang")->wield();
	carry_object("/d/shaolin/obj/xuan-cloth")->wear();
	carry_object("/d/shaolin/obj/sengxie")->wear();
}
