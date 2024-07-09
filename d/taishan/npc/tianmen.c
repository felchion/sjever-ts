inherit NPC;

#include <ansi.h>;
void create()
{
        object ob;
        set_name("天门道人", ({ "tianmen daoren", "daoren", "tianmen" }) );
        set("gender", "男性");
        set("title", HIW"泰山派第十三代掌门"NOR);
        set("class", "taoist");
        set("age", 45);
        set("attitude", "peaceful");
        set("unique", 1);
        set("str", 26);
        set("con", 30);
        set("dex", 30);
        set("int", 28);

        set("neili", 3400);
        set("max_neili", 3400);
        set("jiali", 30);
        set("max_qi",2200);
        set("max_jing",1200);
        set("combat_exp", 1450000);
        set("eff_jingli",2000);
        set("shen_type", 1);
        set("inquiry", ([
                "左冷禅" : "这厮想让五岳同盟，是想让我们帮他效命罢了。。\n",
                "东灵道人" : "大胆，你尽干直呼我祖师爷的名字，\n",
        ]));
        set("chat_chance", 3);
        set("chat_msg", ({
         "天门道人怒不可遏，大声道：“我只是一时的气话，本派掌门人之位，岂能如此草草……草草传授。\n",
         "天门道人叹息道：象玉玑子这种人，我是万万不能传授他为掌门。\n",
         "天门道人怒吼：“你们真要打？那就来拚个你死我活。\n",
        (: command("sneer") :),
        (: command("pei") :),
        }) ); 


        set_skill("sword", 160);
        set_skill("force", 160);
        set_skill("parry", 160);
        set_skill("dodge", 160);
        set_skill("cuff", 160);
        set_skill("taiji-shengong", 160);
        set_skill("taizu-quan", 160);
        set_skill("taishan-jianfa", 160);
        set_skill("qingmang-jian", 160);

        map_skill("sword", "taishan-jianfa");
        map_skill("parry", "taishan-jianfa");
        map_skill("dodge", "qingmang-jian");
        map_skill("force","taiji-shengong");
        map_skill("cuff","taizu-quan");
        prepare_skill("cuff", "taizu-quan");

        create_family("泰山派", 13, "掌门");
        setup();
        if (clonep()) {
                ob = unew(BINGQI_D("sword/dongling_jian"));
                if (!ob) ob = new(BINGQI_D("changjian"));
                ob->move(this_object());
                ob->wield();
                carry_object("/d/wudang/obj/white-robe")->wear();
        }
}

void attempt_apprentice(object ob)
{
    command("shake");
    command("say 泰山派现弟子近二三百人，贫道我暂不想收徒。");
    return;
}

