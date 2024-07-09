// Npc: /kungfu/class/shaolin/cheng-xing.c
// Date: YZC 96/01/19

inherit NPC;
inherit F_MASTER;


void create()
{
	set_name("澄行", ({
		"chengxing luohan",
		"chengxing",
		"xianshan",
		"luohan",
	}));
	set("long",
		"他是一位须发花白的老僧，身穿一袭金边黑布袈裟。他身材瘦高，\n"
		"太阳穴高高鼓起，似乎身怀绝世武功。\n"
	);

	set("nickname", "贤善罗汉");
	set("gender", "男性");
	set("attitude", "friendly");
	set("class", "bonze");

	set("age", 60);
	set("shen", 5000);
	set("str", 25);
	set("int", 25);
	set("con", 25);
	set("dex", 25);
	set("max_qi", 2500);
	set("max_jing", 1000);
	set("neili", 2500);
	set("max_neili", 2500);
	set("jiali", 100);
	set("combat_exp", 700000);
    set("unique", 1);

	set_skill("force", 85);
	set_skill("yijin-jing", 120);
	set_skill("dodge", 85);
	set_skill("shaolin-shenfa", 85);
	set_skill("blade", 90);
	set_skill("cibei-dao", 95);
	set_skill("parry", 85);
        set_skill("cuff", 120);
        set_skill("jingang-quan", 120);
	set_skill("buddhism", 85);
	set_skill("literate", 85);

	map_skill("force", "yijin-jing");
	map_skill("dodge", "shaolin-shenfa");
	map_skill("blade", "cibei-dao");
	map_skill("parry", "cibei-dao");
        map_skill("cuff", "jingang-quan");
        prepare_skill("cuff", "jingang-quan");
        set("chat_chance_combat", 30);
	set("chat_msg_combat", ({
		(: perform_action, "cuff.fumo" :)
	}));

	create_family("少林派", 37, "弟子");

	setup();

        carry_object("/d/shaolin/obj/jiedao")->wield();
        carry_object("/d/shaolin/obj/cheng-cloth")->wear();
}


#include "/kungfu/class/shaolin/cheng.h"
