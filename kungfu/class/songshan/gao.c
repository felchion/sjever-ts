// gao.c 嵩山派第三代弟子 高克新
// Lklv 2001.10.18 Modified

#include <ansi.h>
inherit NPC;

void create()
{
	set_name("高克新", ({ "gao kexin", "gao", "kexin" }));
	set("nickname", HIY"锦毛狮"NOR);
	set("long", "这是一名瘦削的老者，与众不同的是他那金黄的头发，江湖送名“锦毛狮”。\n");
	set("gender", "男性");
	set("age", 45);
	set("per", 20);
	set("attitude", "heroism");
	set("shen_type", -1);
        set("str", 26);
        set("int", 26);
        set("con", 26);
        set("dex", 27);
        set("unique", 1);

        set("max_qi", 3000);
        set("max_jing", 2000);
        set("eff_jingli", 2000);
        set("jiali", 50);
        set("combat_exp", 2000000);
        set("eff_neili", 4000);
        set("shen", -15000);

	set_skill("literate", 100);
	set_skill("force", 160);
	set_skill("parry", 160);
	set_skill("sword", 160);
	set_skill("dodge", 160);
	set_skill("hand", 160);
	set_skill("zhongyuefeng", 160);
	set_skill("songyang-shou", 160);
	set_skill("hanbing-zhenqi", 160);
	set_skill("songshan-jian", 160);

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