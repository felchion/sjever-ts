// /kungfu/class/emei/jing-xuan.c
// Make by Beyond 98.2
// Lklv 2001.10.18 update

#include "nigu.h"
inherit F_MASTER;
inherit NPC;

void create()
{
	set_name("静玄师太", ({
		"jingxuan shitai",
		"jingxuan",
		"shitai",
	}));
	set("long",
		"她就是峨嵋派大弟子静玄师太，在八大静字辈弟子中排行第一，最擅刀法。\n"
		"她身材高大，神态威猛，虽是女子，却比寻常男子还高了半个头。\n"
	);

	set("gender", "女性");
	set("attitude", "friendly");
	set("unique", 1);
	set("class", "bonze");

	set("age", 50);
	set("shen_type", 1);
	set("str", 32);
	set("int", 32);
	set("con", 32);
	set("dex", 32);
	set("max_qi", 1500);
	set("max_jing", 1500);
	set("neili", 1500);
	set("max_neili", 1500);
	set("jiali", 30);
	set("combat_exp", 1000000);
	set("score", 100);
	set("inquiry",([
                "剃度"  : (: ask_for_join :),
                "出家"  : (: ask_for_join :),
        ]));
	set_skill("dacheng-fofa", 130);
	set_skill("linji-zhuang", 130);
	set_skill("yanxing-daofa", 130);
	set_skill("literate", 120);
	set_skill("blade", 130);
	set_skill("parry", 120);
	set_skill("dodge", 120);
	set_skill("force", 120);
	set_skill("anying-fuxiang", 120);

	map_skill("force", "linji-zhuang");
	map_skill("blade", "yanxing-daofa");
	map_skill("parry", "huifeng-jian");
	map_skill("dodge", "anying-fuxiang");

	create_family("峨嵋派", 4, "弟子");

	setup();
	carry_object("/d/emei/obj/blade")->wield();
	carry_object("/d/emei/obj/shoes")->wear();
	carry_object("/d/emei/obj/cloth")->wear();
}
