// /kungfu/class/emei/li-mingxia.c
// Lklv 2001.10.18 update

#include "sujia.h"
inherit F_MASTER;
inherit NPC;

void create()
{
	set_name("李明霞", ({"li mingxia", "li"}));
	set("long", "她是灭绝师太的八大俗家第子之一。\n");

	set("gender", "女性");
	set("attitude", "friendly");
	set("unique", 1);
	set("age", 35);
	set("shen_type", 1);
	set("str", 30);
	set("int", 30);
	set("con", 30);
	set("dex", 33);
	set("max_qi", 1500);
	set("max_jing", 1500);
	set("neili", 1500);
	set("max_neili", 1500);
	set("jiali", 30);
	set("combat_exp", 500000);
	set("score", 100);

	set_skill("dacheng-fofa", 130);
	set_skill("linji-zhuang", 130);
	set_skill("strike", 120);
	set_skill("sixiang-zhang", 120);
	set_skill("literate", 130);
	set_skill("parry", 120);
	set_skill("dodge",120);
	set_skill("force", 130);
	set_skill("anying-fuxiang", 120);

	map_skill("strike", "sixiang-zhang");
	map_skill("force", "linji-zhuang");
	map_skill("parry", "huifeng-jian");
	map_skill("dodge", "anying-fuxiang");

	prepare_skill("strike", "sixiang-zhang");

	create_family("峨嵋派", 4, "弟子");

	setup();
	carry_object("/d/emei/obj/shoes")->wear();
	carry_object("/d/emei/obj/cloth")->wear();
}
