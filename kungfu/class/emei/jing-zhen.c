// /kungfu/class/emei/jing-zhen.c
// Update by Lklv 2001.10.18

#include "nigu.h"
inherit F_MASTER;
inherit NPC;

void create()
{
	set_name("静真师太", ({"jingzhen shitai", "jingzhen", "shitai"}));
	set("long",
		"她是一位慈祥的年轻师太，在福寿庵修行。\n"
		"她是灭绝师太的八大静字辈弟子之八。\n"
	);

	set("gender", "女性");
	set("attitude", "friendly");
	set("unique", 1);
	set("class", "bonze");

	set("age", 25);
	set("shen_type", 1);
	set("str", 30);
	set("int", 30);
	set("con", 30);
	set("dex", 32);
	set("max_qi", 1500);
	set("max_jing", 1500);
	set("neili", 1500);
	set("max_neili", 1500);
	set("jiali", 30);
	set("combat_exp", 500000);
	set("score", 100);

	set_skill("dacheng-fofa", 120);
	set_skill("literate", 120);
	set_skill("linji-zhuang", 120);
	set_skill("huifeng-jian", 110);
	set_skill("sword", 110);
	set_skill("strike", 120);
	set_skill("sixiang-zhang", 120);
	set_skill("parry", 110);
	set_skill("dodge", 110);
	set_skill("force", 120);
	set_skill("anying-fuxiang", 120);

	map_skill("strike", "sixiang-zhang");
	map_skill("force", "linji-zhuang");
	map_skill("sword", "huifeng-jian");
	map_skill("parry", "huifeng-jian");
	map_skill("dodge", "anying-fuxiang");

	prepare_skill("strike", "sixiang-zhang");
	create_family("峨嵋派", 4, "弟子");
	setup();
	carry_object("/d/emei/obj/changjian")->wield();
	carry_object("/d/emei/obj/shoes")->wear();
	carry_object("/d/emei/obj/cloth")->wear();
}
