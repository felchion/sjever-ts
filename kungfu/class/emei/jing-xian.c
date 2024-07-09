// /kungfu/class/emei/jing-xian.c
// Update by Lklv 2001.10.18

#include "nigu.h"
inherit F_MASTER;
inherit NPC;

void create()
{
	set_name("静闲师太", ({"jingxian shitai", "jingxian", "shitai"}));
	set("long",
		"她是一位慈祥的年轻师太，在清音阁修行。\n"
		"她是灭绝师太的八大静字辈弟子之七。\n"
	);

	set("gender", "女性");
	set("attitude", "friendly");
	set("unique", 1);
	set("class", "bonze");

	set("age", 35);
	set("shen_type", 1);
	set("str", 30);
	set("int", 30);
	set("con", 30);
	set("dex", 32);
	set("max_qi", 2000);
	set("max_jing", 2000);
	set("neili", 2000);
	set("max_neili",2000);
	set("jiali", 40);
	set("combat_exp", 700000);
	set("score", 10000);

	set_skill("dacheng-fofa", 135);
	set_skill("linji-zhuang", 135);
	set_skill("literate", 130);
	set_skill("huifeng-jian", 135);
	set_skill("sword", 135);
	set_skill("parry", 135);
	set_skill("blade", 135);
	set_skill("yanxing-daofa", 135);
	set_skill("sixiang-zhang", 135);
	set_skill("dodge", 135);
	set_skill("strike", 135);
	set_skill("force", 135);
	set_skill("anying-fuxiang", 135);
	set("inquiry",([
                "剃度"  : (: ask_for_join :),
                "出家"  : (: ask_for_join :),
        ]));

	map_skill("strike", "sixiang-zhang");
	map_skill("force", "linji-zhuang");
	map_skill("sword", "huifeng-jian");
	map_skill("parry", "huifeng-jian");
	map_skill("dodge", "anying-fuxiang");

	prepare_skill("strike", "sixiang-zhang");

	create_family("峨嵋派", 4, "弟子");
	setup();

	carry_object("/d/emei/obj/shoes")->wear();
	carry_object("/d/emei/obj/cloth")->wear();
}
