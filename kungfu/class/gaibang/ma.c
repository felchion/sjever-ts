// ma.c 马舵主

#include <ansi.h>
inherit NPC;
inherit F_MASTER;

#include "begger.h";

void create()
{
        set_name("马舵主", ({"ma duozhu", "ma", "duozhu"}));
        set("title", YEL"丐帮大勇分舵舵主"NOR);
        set("gb/bags",8);
        set("gender", "男性");
        set("age", 40);
        set("long","他是八袋舵主，掌管大勇分舵。\n");
        set("attitude", "peaceful");
        set("unique", 1);

        set("str", 25);
        set("int", 18);
        set("con", 20);
        set("dex", 20);

        set("qi", 1200);
        set("max_qi", 1200);
        set("jing", 800);
        set("max_jing", 800);
        set("eff_jingli", 940);
        set("neili", 1400);
        set("max_neili", 1400);
        set("jiali", 50);

        set("combat_exp", 320000);

        set_skill("force", 90); // 基本内功
        set_skill("huntian-qigong", 90); // 混天气功
        set_skill("dodge", 90); // 基本躲闪
        set_skill("xiaoyaoyou", 100); // 千里行乞
        set_skill("parry", 100); // 基本招架
        set_skill("staff", 100);
        set_skill("fengmo-zhang",100);
        set_skill("lianhua-zhang",100);
        set_skill("strike",100);
        set_skill("begging", 90);

        map_skill("force", "huntian-qigong");
        map_skill("parry","fengmo-zhang");
        map_skill("staff","fengmo-zhang");
        map_skill("dodge", "xiaoyaoyou");
        map_skill("strike", "lianhua-zhang");
        prepare_skill("strike", "lianhua-zhang");

        create_family("丐帮", 19, "八袋弟子");

        set("inquiry", ([
                "丐帮" : "我们丐帮是天下第一大帮！\n",
                "萧峰" : "他不就是乔峰，我们的前任帮主。\n",
                "学掌法" : "那都是很多年前的事了，他那时常到附近的一个小岛\n"
                           "上埋头苦练。\n",
        ]));
        setup();

        carry_object(__DIR__"obj/gb_budai8")->wear();
        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
        carry_object(BINGQI_D("gangzhang"))->wield();
}

void attempt_apprentice(object ob)
{

	if ((int)ob->query_str() <=20	) {
		command("say 你膂力不足，恐怕学不了我的功夫。\n");
		return;
	}

        if ((int)ob->query("gb/bags") >= 2
        && ob->query("family/family_name") != "丐帮" ) {
                command("say 丐帮可容不下你这种背信弃义的小人！");
                command("kill "+ob->query("id"));
                return;
        }

        command("say 好吧，希望" + RANK_D->query_respect(ob) +
        "能好好学习本门武功，将来在江湖中闯出一番作为。");
	command("recruit " + ob->query("id"));

        if (!ob->query("gb/bags")) ob->set("gb/bags", 1);
        ob->set("title",sprintf("丐帮大勇分舵%s袋弟子",
                chinese_number(ob->query("gb/bags"))) );
        ob->set("gb/fenduo","大勇分舵");
        ob->set("class","beggar");
}
