// /kungfu/class/nshaolin/yuantong.c
// looklove 2001.9.12

inherit NPC;
inherit F_MASTER;

void create()
{
        set_name("元痛尊者", ({
                "yuantong zunzhe",
                "yuantong",
                "zunzhe",
        }));
        set("long",
                "一位脾气暴躁的中年僧人，身材很是肥胖。\n"
        );

        set("gender", "男性");
        set("attitude", "friendly");
        set("class", "bonze");

        set("age", 32);
        set("shen_type", 1);
        set("str", 30);
        set("int", 30);
        set("con", 30);
        set("dex", 30);
        set("max_qi", 2000);
        set("max_jing", 2000);
        set("neili", 6000);
        set("max_neili", 4000);
        set("jiali", 150);
        set("combat_exp", 1200000);

        set_skill("force", 180);
        set_skill("yijin-jing", 180);
        set_skill("dodge", 180);
        set_skill("shaolin-shenfa", 180);
        set_skill("parry", 180);
        set_skill("claw", 180);
        set_skill("staff", 180);
        set_skill("finger", 180);
        set_skill("longzhua-gong", 180);
        set_skill("pudu-zhang", 180);
        set_skill("yizhi-chan", 180);
        set_skill("medicine", 120);
        set_skill("buddhism", 200);
        set_skill("literate", 140);

        map_skill("force", "yijin-jing");
        map_skill("dodge", "shaolin-shenfa");
        map_skill("claw", "longzhua-gong");
        map_skill("finger", "yizhi-chan");
        map_skill("staff", "pudu-zhang");
        map_skill("parry", "yizhi-chan");

        prepare_skill("claw", "longzhua-gong");
        prepare_skill("finger", "yizhi-chan");

	set("chat_chance_combat", 60);
	set("chat_msg_combat", ({
		(: perform_action, "finger.qiankun" :),
		(: perform_action, "finger.wofo" :),
		(: perform_action, "claw.canyun" :),
	}));

        create_family("南少林", 23, "弟子");

        setup();

	carry_object("/d/putian/npc/obj/fb-chan")->wield();
	carry_object("/d/shaolin/obj/cheng-cloth")->wear();
	carry_object("/d/shaolin/obj/sengxie")->wear();
}
