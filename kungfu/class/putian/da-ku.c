// /kungfu/class/nshaolin/da-ku.c
// looklove 2001.7.21

inherit NPC;
inherit F_MASTER;

void create()
{
        object ob;
        set_name("大苦大师", ({
                "daku dashi",
                "daku",
                "dashi",
        }));
        set("long",
                "他面色红润，留着一缕花白的胡须，显得神采熠熠。\n"
        );

        set("gender", "男性");
        set("attitude", "friendly");
        set("class", "bonze");
	set("title", "莆田少林寺监寺");

        set("age", 54);
        set("shen_type", 1);
        set("str", 40+random(10));
        set("int", 30);
        set("con", 30);
        set("dex", 30);
        set("max_qi", 2800);
        set("max_jing", 2000);
        set("neili", 6000);
        set("max_neili", 4000);
        set("jiali", 100);
        set("combat_exp", 2400000);

        set_skill("force", 200);
        set_skill("yijin-jing", 200);
        set_skill("dodge", 200);
        set_skill("finger", 200);
        set_skill("parry", 200);
        set_skill("cuff", 200);
        set_skill("club", 200);
        set_skill("luohan-quan", 200);
        set_skill("zuibaxian", 200);
        set_skill("buddhism", 190);
        set_skill("literate", 120);
        set_skill("zui-gun",200);
        set_skill("wuxiang-zhi", 200);

        map_skill("force", "yijin-jing");
        map_skill("dodge", "zuibaxian");
        map_skill("parry", "zui-gun");
        map_skill("cuff", "luohan-quan");
	map_skill("finger", "wuxiang-zhi");
        map_skill("club", "zui-gun");

        prepare_skill("cuff", "luohan-quan");
        prepare_skill("finger", "wuxiang-zhi");

	set("chat_chance_combat", 60);
	set("chat_msg_combat", ({
		(: perform_action, "finger.qiankun" :),
		(: perform_action, "club.zuijiu" :),
	}));

        create_family("南少林", 22, "弟子");

        setup();

        if (clonep()) {
                ob = unew(BINGQI_D("banzhu-gun"));
                if (!ob) ob = unew("/d/shaolin/obj/qimeigun");
                ob->move(this_object());
                ob->wield();
                carry_object("/d/shaolin/obj/xuan-cloth")->wear();
                carry_object("/d/putian/npc/obj/sengxie")->wear();
        }
}
