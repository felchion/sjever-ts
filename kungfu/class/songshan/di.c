// di.c 嵩山派第四代弟子 狄修
// Lklv 2001.10.18 Modified

#include <ansi.h>
inherit NPC;

void create()
{
        set_name("狄修", ({ "di xiu", "di", "xiu" }));
        set("long", "这是一名精壮的汉子，是左冷禅的徒弟。\n");
        set("gender", "男性");
        set("age", 30);
        set("per", 20);
        set("attitude", "heroism");
        set("shen_type", -1);
        set("str", 23);
        set("int", 23);
        set("con", 23);
        set("dex", 24);
        set("unique", 1);

        set("max_qi", 1500);
        set("max_jing", 1000);
        set("eff_jingli", 1000);
        set("jiali", 20);
        set("combat_exp", 700000);
        set("shen", -7000);

	set_skill("force", 110);
	set_skill("hanbing-zhenqi", 110);
	set_skill("dodge", 100);
	set_skill("zhongyuefeng", 100);
	set_skill("hand", 100);
	set_skill("songyang-shou", 100);
	set_skill("parry", 100);
	set_skill("sword", 100);
	set_skill("songshan-jian", 100);
	set_skill("literate", 60);

	map_skill("force", "hanbing-zhenqi");
	map_skill("dodge", "zhongyuefeng");
	map_skill("hand", "songyang-shou");
	map_skill("parry", "songshan-jian");
	map_skill("sword", "songshan-jian");

	prepare_skill("hand", "songyang-shou");

	create_family("嵩山派", 4, "弟子");

	set("class", "songshan");
	set("inquiry", ([
		"嵩山派" : "我们嵩山派是五岳剑派之首！\n",
		"五岳剑派" : "我们五岳剑派在武林之中势力巨大，可与少林、武当抗衡。\n",
		"左冷禅" : "他就是我们的掌门人，功夫深不可测。\n",
	]) );
	setup();
	carry_object(ARMOR_D("changpao1"))->wear();
	carry_object(BINGQI_D("changjian"))->wield();
}

#include "app4.h";