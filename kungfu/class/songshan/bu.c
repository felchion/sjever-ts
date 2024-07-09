// bu.c 嵩山派第三代弟子 卜沉
// Creat by Lklv 2001.10.18

#include <ansi.h>
inherit NPC;

void create()
{
        set_name("卜沉", ({ "bu chen", "bu", "chen" }));
        set("nickname", WHT"白头仙翁"NOR);
        set("long", "这是一名满头白发的老者。\n");
        set("gender", "男性");
        set("unique", 1);
        set("age", 45);
        set("per", 20);
        set("attitude", "heroism");
        set("shen_type", -1);
        set("str", 20);
        set("int", 20);
        set("con", 20);
        set("dex", 20);
        set("unique", 1);

        set("max_qi", 5000);
        set("max_jing", 4000);
        set("eff_jingli", 4000);
        set("jiali", 50);
        set("combat_exp", 3000000);
        set("eff_neili", 4000);
        set("shen", -15000);

	set_skill("literate", 200);
	set_skill("force", 250);
	set_skill("parry", 240);
	set_skill("sword", 250);
	set_skill("dodge", 250);
	set_skill("hand", 250);
	set_skill("strike", 250);
	set_skill("zhongyuefeng", 250);
	set_skill("songyang-shou", 250);
	set_skill("hanbing-zhenqi", 250);
	set_skill("songshan-jian", 250);

	map_skill("force", "hanbing-zhenqi");
	map_skill("dodge", "zhongyuefeng");
	map_skill("hand", "songyang-shou");
	map_skill("parry", "songshan-jian");
	map_skill("sword", "songshan-jian");

	prepare_skill("hand", "songyang-shou");

	create_family("嵩山派", 3, "弟子");
	set("class", "songshan");
	set("chat_chance_combat", 10);
	set("chat_msg_combat", ({
		(: perform_action, "sword.feiwu" :),
		(: perform_action, "hand.junji" :),
                (: exert_function, "juhan" :),
	}));
	set("inquiry", ([
		"嵩山派" : "我们嵩山派是五岳剑派之首！\n",
		"五岳剑派" : "我们五岳剑派在武林之中势力巨大，可与少林、武当抗衡。\n",
		"左冷禅" : "他就是我们的掌门人，功夫深不可测。\n",
	]) );
        setup();
        carry_object(ARMOR_D("changpao1"))->wear();
        carry_object(BINGQI_D("changjian"))->wield();
}

#include "app3.h";
