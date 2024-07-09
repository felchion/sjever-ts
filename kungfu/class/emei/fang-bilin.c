// /kungfu/class/emei/fang-bilin.c
// Make by Jackie 98.2

#include "sujia.h"
inherit NPC;
inherit F_MASTER;
void create()
{
	set_name("方碧琳", ({
		"fang bilin",
		"fang",
	}));
	set("long",
	"她是灭绝师太的八大俗家第子之一。\n"
	);


	set("gender", "女性");
	set("attitude", "friendly");
	set("unique", 1);
	set("age", 25);
	set("shen_type", 1);
	set("str", 30);
	set("int", 30);
	set("con", 30);
	set("dex", 30);
	set("max_qi", 800);
	set("max_jing", 800);
	set("neili", 800);
	set("max_neili", 800);
	set("jiali", 8);
	set("combat_exp", 200000);
	set("score", 100);

	set_skill("dacheng-fofa", 130);
	set_skill("linji-zhuang", 130);
	set_skill("huifeng-jian", 130);
	set_skill("sword", 120);
	set_skill("literate", 130);
	set_skill("parry", 120);
	set_skill("dodge", 120);
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



